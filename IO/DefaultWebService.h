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

  public:
    DefaultWebService(/* args */) : WebService() {}
    
    /*virtual*/ bool canExecute(HttpCommand &command)
    {
        return true;
    }
};

#endif