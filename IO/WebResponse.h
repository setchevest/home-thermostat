#ifndef WebResponse_h
#define WebResponse_h

#include <Ethernet.h>
#include <Common/Serializable.h>

enum HttpVerb
{
    GET = 1,
    POST = 2,
    PUT = 3,
    Unknown
};

struct HttpCommand
{
    bool completed = false;
    HttpVerb verb = HttpVerb::Unknown;
    const char *route = "";
    void printTo(Print &printer)
    {
#ifdef LOGGING
        printer.print(F("Command: "));
        printer.print(F(" verb "));
        printer.print(verb == 1 ? "GET" : (verb == 2 ? "POST" : (verb == 3 ? "PUT" : "Unknown")));
        printer.print(F(" route /"));
        printer.println(route);
#endif
    }
};

class WebResponse
{
  protected:
    virtual void addHeader(Print &client)
    {
        // send a standard http response header
        client.println(F("HTTP/1.1 200 OK"));
        client.println(getContentType());
        client.println(F("Connection: close")); // close after completion of the response
        client.println();                       // end of HTTP header
    }

    virtual void addBody(Print &client) = 0;

    virtual const char *getContentType() = 0;

  public:
    WebResponse() {}
    ~WebResponse() {}

    //Template Method
    void flush(Print &client)
    {
        addHeader(client);
        addBody(client);
    }

    //Template Method
    void bodyFlush(Print &client)
    {
        addBody(client);
    }

};

#endif