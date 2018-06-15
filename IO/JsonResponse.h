#ifndef JsonResponse_h
#define JsonResponse_h

#include <Ethernet.h>
#include <Common/Serializable.h>
#include <IO/WebResponse.h>

#ifndef JSON_BUFFER_SIZE
#define JSON_BUFFER_SIZE 120
#endif

class JsonResponse : public WebResponse
{
private:
  Serializable &body;
  StaticJsonBuffer<JSON_BUFFER_SIZE> jsonBuffer;

protected:
  /*virtual*/ const char *getContentType()
  {
    return "Content-Type: application/json";
  }

  /*virtual*/ void addBody(Client &client)
  {

    JsonObject &root = jsonBuffer.createObject();
    body.toJson(root);
    root.printTo(client);
  }

public:
  JsonResponse(Serializable &body_) : body(body_), WebResponse() {}
  ~JsonResponse() {}
};

#endif