#ifndef EthernetClient_h
#define EthernetClient_h
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Common/Serializable.h>
#include <Common/Interfaces/IObserver.h>
#include <Configuration.h>
#include <IO/WebServiceRegistry.h>

// the maximum length of paramters we accept
class Esp8266CommandHandler : public IObserver
{
  private:
    ESP8266WebServer server; //Used for incomming request
    Configuration::EthernetConfiguration &config;
    Configuration::WifiNetworkConfiguration &wifiConfig;

    void processRequest(const char* route)
    {
        HttpCommand command;
        command.completed = true;
        command.verb = HttpVerb::GET;
        command.route = route;
        WiFiClient c = server.client();
        WebServiceRegistry::getInstance().getService(command)->processRequest(command, c);
    }

  public:
    Esp8266CommandHandler(Configuration::EthernetConfiguration &_config, Configuration::WifiNetworkConfiguration &wifiConfig_)
        : config(_config), wifiConfig(wifiConfig_),  
            server(IPAddress(_config.defaultIp[0], _config.defaultIp[1], _config.defaultIp[2], _config.defaultIp[3]), _config.port) 
            {}

    ~Esp8266CommandHandler()
    {
        
    }

    void init()
    {
        WiFi.mode(WIFI_STA);
        WiFi.hostname(wifiConfig.deviceName); 
        WiFi.config(IPAddress(config.defaultIp[0], config.defaultIp[1], config.defaultIp[2], config.defaultIp[3]),
            IPAddress(config.gateway[0], config.gateway[1], config.gateway[2], config.gateway[3]),
            IPAddress(config.subnet[0], config.subnet[1], config.subnet[2], config.subnet[3])); 
        WiFi.begin(wifiConfig.ssid, wifiConfig.pass);

        // Wait for connection
        while (WiFi.status() != WL_CONNECTED)
        {
            delay(500);
            Serial.print(".");
        }
#ifdef LOGGING
        Serial.println();
        Serial.print(F("server is at "));
        Serial.print(F("http://"));
        Serial.print(WiFi.localIP());
        Serial.print(F(":"));
        Serial.println(config.port);
#endif
        
        server.onNotFound([this]() {
            this->processRequest(server.uri().substring(1).c_str());
        });

        server.begin();
#ifdef LOGGING
        Serial.println("HTTP server started");
#endif
        TickNotifier::getInstance().attach(this);
    }

    void update(const char *command)
    {
        if (strcmp(command, TickNotifier::getInstance().tickCommand) == 0)
        {
            server.handleClient();
        }
    }
};

#endif