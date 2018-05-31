#ifndef EthernetClient_h
#define EthernetClient_h
#include <SPI.h>
#include <Ethernet.h>
#include <Common/Serializable.h>
#include <Common/Interfaces/IObserver.h>
#include <Configuration.h>
#include <IO/NamedCallbackRegistry.h>
#include <IO/JsonResponse.h>

// the maximum length of paramters we accept
const unsigned int MAX_GET_INPUT = 100;
const int MAX_PARAM = 10;

class EthernetCommandHandler : public IObserver
{
  private:
    EthernetServer server; //Used for incomming request
    EthernetClient httpClient; //Used for outgoing request
    Serializable &responseBody;
    Configuration::EthernetConfiguration &config;
    NamedCallbackRegistry commands;
    bool callServerRequested = false;

    // Example GET line: GET /?foo=bar HTTP/1.1
    void processIncommingGet(const char *data)
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
        commands.handle(param);

    } // end of processIncommingGet

    // here to process incoming serial data after a terminator received
    void processData(const char *data)
    {
        if (strlen(data) < 4)
            return;

        if (memcmp(data, "GET ", 4) == 0)
            processIncommingGet(&data[4]);

        if (memcmp(data, "PUT ", 4) == 0)
            processIncommingGet(&data[5]);

        if (memcmp(data, "POST ", 5) == 0)
            processIncommingGet(&data[5]);

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
    }

    void processIncomingRequest()
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

            JsonResponse response(client, responseBody);
            response.flush();
            
            Serial.println(F("Client disconnected"));
        } // end of got a new client
    }     // end of loop

    // this method makes a HTTP connection to the server:
    void httpRequest()
    {
        // close any connection before send a new request.
        // This will free the socket on the WiFi shield
        httpClient.stop();

        // if there's a successful connection:
        if (httpClient.connect(config.hostURL, config.hostPort))
        {
            Serial.println(F("connecting..."));
            // send the HTTP GET request:
            httpClient.println("GET /search?q=arduino HTTP/1.1");
            httpClient.println("Host: www.google.com");
            httpClient.println("User-Agent: arduino-ethernet");
            httpClient.println("Connection: close");
            httpClient.println();
            Serial.println(F("Finished."));
        }
        else
        {
            // if you couldn't make a connection:
            Serial.println(F("connection failed"));
        }
    }

  public:
    EthernetCommandHandler(Configuration::EthernetConfiguration &_config, Serializable &_responseBody)
        : config(_config), responseBody(_responseBody), server(_config.port), commands(NamedCallbackRegistry()) {}

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
            Ethernet.begin(config.defaultMac,IPAddress(config.defaultIp[0],config.defaultIp[1],config.defaultIp[2],config.defaultIp[3]));
        }
        server.begin();
        Serial.print(F("server is at "));
        Serial.println(Ethernet.localIP());
        TickNotifier::getInstance().attach(this);
    }

    void update(const char *command)
    {
        if (strcmp(command, TickNotifier::getInstance().tickCommand) == 0)
        {
            processIncomingRequest();
            if (callServerRequested)
            {
                Serial.println(F("calling server"));
                callServerRequested = false;
                httpRequest();
            }
            else
            {
                if (httpClient.available())
                {
                    Serial.println(F("Getting data from server"));
                    char c;
                    while (httpClient.available())
                    {
                        c = httpClient.read();
                        Serial.print(c);
                    }
                }
            }
        }
    }

    void callServerAsync()
    {
        Serial.println(F("Call Server requested."));
        callServerRequested = true;
    }
};

#endif