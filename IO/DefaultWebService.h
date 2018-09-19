#ifndef DefaultWebService_h
#define DefaultWebService_h
#include <IO/WebService.h>
#include <IO/EmptyWebResponse.h>

class DefaultWebService : public WebService
{
  private:

  protected:
    /*virtual*/ ActionResponse &executeCommand(HttpCommand &command)
    {
        static EmptyWebResponse response("Wrong command.");
        return response;
    }

  public:
    DefaultWebService(/* args */) : WebService() {}

    /*virtual*/ bool canExecute(HttpCommand &command)
    {
        return true;
    }
};

#endif