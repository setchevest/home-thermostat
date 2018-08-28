#ifndef WebService_h
#define WebService_h
#include <IO/WebService.h>

class WebService
{
protected:
  virtual void executeCommand(HttpCommand &command, Client &client) = 0;

  virtual void executeMessage(char *topic, byte *payload, unsigned int length) = 0;
  
  static bool isRoute(const char *route, const char *validRoute)
  {
    return strcmp(route, validRoute) == 0;
  }

public:
  virtual bool canExecute(HttpCommand &command) = 0;

  virtual bool canExecute(const char *topic) = 0;

  virtual void processRequest(HttpCommand &command, Client &client)
  {
#ifdef LOGGING
    command.printTo(Serial);
#endif
    executeCommand(command, client);
    delay(2);      //let browser get the info.
    client.stop(); //Close the connection
  }

  virtual void processMessage(char *topic, byte *payload, unsigned int length)
  {

#ifdef LOGGING
    Serial.print(topic);
    Serial.print(F(" - "));
    for (size_t i = 0; i < length; i++)
    {
      Serial.print((char)payload[i]);
    }
    Serial.println();
#endif
    executeMessage(topic, payload, length);
  }
};

#endif