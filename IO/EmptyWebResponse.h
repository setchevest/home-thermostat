#ifndef EmptyWebResponse_h
#define EmptyWebResponse_h

#include <IO/ActionResponse.h>

#define TEXTPLAIN_CONTENTTYPE "Content-Type: text/plain"

class EmptyWebResponse : public ActionResponse
{
  public:
    EmptyWebResponse(const char* responseBody_ = "") : ActionResponse(), responseBody(responseBody_) {}
    ~EmptyWebResponse() {}

  protected:
    const char* responseBody;

    /*virtual*/ const char* getContentType()
    {
        return TEXTPLAIN_CONTENTTYPE;
    }

    /*virtual*/ void addBody(Print &client)
    {
        client.println(responseBody);
    }
};

#endif