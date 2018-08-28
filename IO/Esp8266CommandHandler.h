#define LOGGING
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
#include <PubSubClient.h>

// the maximum length of paramters we accept
class Esp8266CommandHandler : public IObserver
{
  private:
    ESP8266WebServer server; //Used for incomming request
    Configuration::WifiNetworkConfiguration &wifiConfig;
    Configuration::ServerConfig &serverConfig;
    long lastReconnectAttempt = 0;

    void processRequest(const char *route)
    {
        HttpCommand command;
        command.completed = true;
        command.verb = HttpVerb::GET;
        command.route = route;
        WiFiClient c = server.client();

        WebServiceRegistry::getInstance().getService(command)->processRequest(command, c);
    }

  public:
    Esp8266CommandHandler(Configuration::WifiNetworkConfiguration &wifiConfig_, Configuration::ServerConfig &serverConfig_)
        : wifiConfig(wifiConfig_), serverConfig(serverConfig_),
          server(IPAddress(wifiConfig_.defaultIp[0], wifiConfig_.defaultIp[1], wifiConfig_.defaultIp[2], wifiConfig_.defaultIp[3]), wifiConfig_.port)
    {
    }

    ~Esp8266CommandHandler()
    {
    }

    void init()
    {
        WiFi.mode(WIFI_STA);
        WiFi.hostname(wifiConfig.deviceName);
        WiFi.config(IPAddress(wifiConfig.defaultIp[0], wifiConfig.defaultIp[1], wifiConfig.defaultIp[2], wifiConfig.defaultIp[3]),
                    IPAddress(wifiConfig.gateway[0], wifiConfig.gateway[1], wifiConfig.gateway[2], wifiConfig.gateway[3]),
                    IPAddress(wifiConfig.subnet[0], wifiConfig.subnet[1], wifiConfig.subnet[2], wifiConfig.subnet[3]));
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
        Serial.println(wifiConfig.port);
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