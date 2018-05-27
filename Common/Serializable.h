#ifndef Serializable_h
#define Serializable_h

#include <Arduino.h>
#include <ArduinoJson.h>

class Serializable
{
private:
    /* data */
public:
    Serializable(/* args */) { }
    ~Serializable() { }
    virtual void toJson(JsonObject &root) = 0;
};

#endif