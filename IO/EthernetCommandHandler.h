#ifndef EthernetClient_h
#define EthernetClient_h
#include <SPI.h>
#include <Ethernet.h>
#include <Common/Serializable.h>
#include <Common/Interfaces/IObserver.h>
#include <Configuration.h>

// the maximum length of paramters we accept
const unsigned int MAX_GET_INPUT = 100;
const int MAX_PARAM = 10;

class EthernetCommandHandler : public IObserver
{
  private:
    EthernetServer server;
    Serializable &itemToSerialize;
    Configuration::EthernetConfiguration &config;

    // Example GET line: GET /?foo=bar HTTP/1.1
    void processGet(const char *data)
    {
        Serial.println(data);
        // find where the parameters start
        const char *paramsPos = strchr(data, '/');
        if (paramsPos == NULL)
            return; // no parameters
        // find the trailing space
        const char *spacePos = strchr(paramsPos, ' ');
        if (spacePos == NULL)
            return; // no space found
        // work out how long the parameters are
        int paramLength = spacePos - paramsPos - 1;
        // see if too long
        if (paramLength >= MAX_PARAM)
            return; // too long for us
        // copy parameters into a buffer
        char param[MAX_PARAM];
        memcpy(param, paramsPos + 1, paramLength); // skip the "/"
        param[paramLength] = 0;                    // null terminator

        // do things depending on argument (GET parameters)

        if (strcmp(param, "info") == 0)
        {
            
        }
        else if (strcmp(param, "bar") == 0)
            Serial.println(F("Activating bar"));

    } // end of processGet

    // here to process incoming serial data after a terminator received
    void processData(const char *data)
    {
        if (strlen(data) < 4)
            return;

        if (memcmp(data, "GET ", 4) == 0)
            processGet(&data[4]);
        
        if (memcmp(data, "PUT ", 4) == 0)
            processGet(&data[5]);

        if (memcmp(data, "POST ", 5) == 0)
            processGet(&data[5]);

        
    } // end of processData

    bool processIncomingByte(const byte inByte)
    {
        static char input_line[MAX_GET_INPUT];
        static unsigned int input_pos = 0;
        switch (inByte)
        {
        case '\n':                     // end of text
            input_line[input_pos] = 0; // terminating null byte
            if (input_pos == 0)
                return true; // got blank line
            // terminator reached! process input_line here ...
            processData(input_line);
            // reset buffer for next time
            input_pos = 0;
            break;
        case '\r': // discard carriage return
            break;

        default:
            // keep adding if not full ... allow for terminating null byte
            if (input_pos < (MAX_GET_INPUT - 1))
                input_line[input_pos++] = inByte;
            break;
        }             // end of switch
        return false; // don't have a blank line yet
    }                 // end of processIncomingByte

    void processRequest()
    {
        // listen for incoming clients
        EthernetClient client = server.available();
        if (client)
        {
            Serial.print(F("Memory:"));
            Serial.println(Environment::getFreeMemory());
            Serial.println(F("Client connected"));
            
            // an http request ends with a blank line
            bool done = false;
            while (client.connected() && !done)
            {
                while (client.available() > 0 && !done)
                    done = processIncomingByte(client.read());
            } // end of while client connected

            // send a standard http response header
            client.println(F("HTTP/1.1 200 OK"));
            client.println(F("Content-Type: application/json"));
            client.println(F("Connection: close")); // close after completion of the response
            client.println();                       // end of HTTP header
            
            StaticJsonBuffer<250> jsonBuffer;
            JsonObject &root = jsonBuffer.createObject();
            itemToSerialize.toJson(root);
            root.printTo(client);

            // give the web browser time to receive the data
            delay(1);
            // close the connection:
            client.stop();
            Serial.println(F("Client disconnected"));
        } // end of got a new client
    }     // end of loop

  public:
    EthernetCommandHandler(Configuration::EthernetConfiguration &_config, Serializable &_itemToSerialize)
        : config(config), itemToSerialize(_itemToSerialize), server(_config.port)
    {
        TickNotifier::getInstance().attach(this);
    }

    ~EthernetCommandHandler() {}

    void init()
    {
        // Start the Ethernet connection and the server
        Serial.println(F("Start configuration using DHCP"));
        if (Ethernet.begin(config.defaultMac) == 0)
        {
            Serial.println(F("Failed to configure Ethernet using DHCP"));
            // no point in carrying on, so do nothing forevermore:
            // try to congifure using IP address instead of DHCP:
            Ethernet.begin(config.defaultMac, config.defaultIp);
        }
        server.begin();
        Serial.print(F("server is at "));
        Serial.println(Ethernet.localIP());
    }

    void update()
    {
        processRequest();
    }
};

#endif