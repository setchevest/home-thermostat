#ifndef WebService_h
#define WebService_h
#include <IO/WebService.h>
#include <IO/ActionResponse.h>

class WebService
{
protected:
  virtual ActionResponse & executeCommand(HttpCommand &command) = 0;
  
  static bool isRoute(const char *route, const char *validRoute)
  {
    return strcmp(route, validRoute) == 0;
  }

public:
  virtual bool canExecute(HttpCommand &command) = 0;

  virtual ActionResponse & processRequest(HttpCommand &command)
  {
#ifdef LOGGING
    command.printTo(Serial);
#endif
    return executeCommand(command);
  }
};

#endif