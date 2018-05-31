#ifndef JsonResponse_h
#define JsonResponse_h

#include <Ethernet.h>
#include <Common/Serializable.h>

class JsonResponse
{
  private:
    Client &client;
    Serializable &body;

    void addHeader()
    {
        // send a standard http response header
        client.println(F("HTTP/1.1 200 OK"));
        client.println(F("Content-Type: application/json"));
        client.println(F("Connection: close")); // close after completion of the response
        client.println();                       // end of HTTP header
    }

    void addBody()
    {
        StaticJsonBuffer<250> jsonBuffer;
        JsonObject &root = jsonBuffer.createObject();
        body.toJson(root);
        root.printTo(client);
    }

  public:
    JsonResponse(Client &client_, Serializable &body_) : client(client_),body(body_) {}
    ~JsonResponse() {}

    void flush()
    {
        addHeader();
        addBody();
        delay(1); //let browser get the info.
        // close the connection:
        client.stop();
    }
};

#endif