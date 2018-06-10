#ifndef EthernetClient_h
#define EthernetClient_h
#include <SPI.h>
#include <Ethernet.h>
#include <Common/Serializable.h>
#include <Common/Interfaces/IObserver.h>
#include <Configuration.h>
#include <IO/HttpCommandParser.h>
#include <IO/WebServiceRegistry.h>

// the maximum length of paramters we accept
class EthernetCommandHandler : public IObserver
{
  private:
    EthernetServer server; //Used for incomming request
    Configuration::EthernetConfiguration &config;
    HttpCommandParser &parser;

    void processRequest()
    {
        EthernetClient client = server.available();
        // listen for incoming client
        if (client)
        {
            Serial.println(F("Client connected"));
            HttpCommand command = parser.parse(client);
            Environment::printDiagnosticData(Serial);
            WebServiceRegistry::getInstance().getService(command)->processRequest(command, client);
            Environment::printDiagnosticData(Serial);
            Serial.println(F("Client disconnected"));
        }
    }

  public:
    EthernetCommandHandler(Configuration::EthernetConfiguration &_config, HttpCommandParser &parser_)
        : config(_config), server(_config.port), parser(parser_) {}

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
            Ethernet.begin(config.defaultMac, IPAddress(config.defaultIp[0], config.defaultIp[1], config.defaultIp[2], config.defaultIp[3]));
        }
        server.begin();
        Serial.print(F("server is at "));
        Serial.print(F("http://"));
        Serial.print(Ethernet.localIP());
        Serial.print(F(":"));
        Serial.println(config.port);
        TickNotifier::getInstance().attach(this);
    }

    void update(const char *command)
    {
        if (strcmp(command, TickNotifier::getInstance().tickCommand) == 0)
        {
            processRequest();
        }
    }
};

#endif