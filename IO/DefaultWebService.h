#ifndef DefaultWebService_h
#define DefaultWebService_h
#include <IO/WebService.h>
#include <IO/EmptyWebResponse.h>

class DefaultWebService : public WebService
{
  protected:
    /*virtual*/ void executeCommand(HttpCommand &command, Client &client)
    {
        EmptyWebResponse response("Wrong command.");
        response.flush(client);
    }

    /*virtual*/ void executeMessage(char *topic, byte *payload, unsigned int length)
    {
#ifdef LOGGING
        Serial.print(topic);
        Serial.print(F(" - "));
        Serial.println(F("No action to be executed."));
#endif
    }

  public:
    DefaultWebService(/* args */) : WebService() {}

    /*virtual*/ bool canExecute(HttpCommand &command)
    {
        return true;
    }

    /*virtual*/ bool canExecute(const char *topic)
    {
        return true;
    }
};

#endif