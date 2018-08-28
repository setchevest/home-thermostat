#ifndef EmptyWebResponse_h
#define EmptyWebResponse_h

#include <IO/WebResponse.h>

#define TEXTPLAIN_CONTENTTYPE "Content-Type: text/plain"

class EmptyWebResponse : public WebResponse
{
  public:
    EmptyWebResponse(const char* responseBody_ = "") : WebResponse(), responseBody(responseBody_) {}
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