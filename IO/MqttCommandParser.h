#ifndef MqttCommandParser_h
#define MqttCommandParser_h
#include <Common/Serializable.h>

#ifndef MAX_PARAM_SIZE
#define MAX_PARAM_SIZE 10
#endif

#ifndef MAX_REQUEST_INPUT
#define MAX_REQUEST_INPUT 200
#endif

class MqttCommandParser
{
  private:
    // // here to process incoming serial data after a terminator received
    static HttpVerb resolveVerb(const char *data)
    {
        HttpVerb result = HttpVerb::Unknown;
        if (strlen(data) < 4)
            return result;

        if (memcmp(data, "set", 3) == 0)
            result = HttpVerb::GET;
        else if (memcmp(data, "get", 3) == 0)
            result = HttpVerb::POST;
        else
            result = HttpVerb::Unknown;
        return result;
    } // end of processData

    static void processIncomingRequest(const char* topic, byte *payload, unsigned int length, HttpCommand *command)
    {
        if (length > MAX_REQUEST_INPUT)
        {
#ifdef LOGGING
            Serial.println("Max payload length exeeded.");
#endif
            command->completed = false;
            return;
        }

        StaticJsonBuffer<MAX_REQUEST_INPUT> jsonBuffer;
        char json[length + 1];
        strncpy(json, (char *)payload, length);
        json[length] = '\0';

        JsonObject &data = jsonBuffer.parseObject((char *)json);
        if (!data.success())
        {
            command->completed = false;
#ifdef LOGGING
            Serial.println("parseObject() failed");
#endif
            return;
        }
        else
        {
            command->completed = true;
        }
        // Check request method
        command->route = data["method"];
        command->verb = resolveVerb(command->route);
        command->params = data["params"];
    }

    static const char *getLastPath(const char *topic)
    {
        return &strrchr(topic, '/')[1];
    }

  public:
    MqttCommandParser(/* args */) {}
    ~MqttCommandParser() {}

    HttpCommand parse(const char* topic, byte *payload, unsigned int length)
    {
        HttpCommand command;
        processIncomingRequest(topic, payload, length, &command);
        return command;
    }

    const char *parseReqId(const char *topic)
    {
        return getLastPath(topic);
    }
};

#endif