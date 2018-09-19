#ifndef JsonResponse_h
#define JsonResponse_h

#include <Ethernet.h>
#include <Common/Serializable.h>
#include <IO/ActionResponse.h>

#ifndef JSON_BUFFER_SIZE
#define JSON_BUFFER_SIZE 300
#endif

#define JSON_CONTENTTYPE "Content-Type: application/json"

class JsonResponse : public ActionResponse
{
private:
  Serializable &body;

protected:
  /*virtual*/ const char *getContentType()
  {
    return JSON_CONTENTTYPE;
  }

  /*virtual*/ void addBody(Print &client)
  {
    StaticJsonBuffer<JSON_BUFFER_SIZE> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    body.toJson(root);
    root.printTo(client);
  }

public:
  JsonResponse(Serializable &body_) : body(body_), ActionResponse() {}
  ~JsonResponse() {}
};

#endif