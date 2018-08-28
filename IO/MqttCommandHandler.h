#define LOGGING
#ifndef EthernetClient_h
#define EthernetClient_h

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <Common/Serializable.h>
#include <Common/Interfaces/IObserver.h>
#include <Common/Interfaces/INotifier.h>
#include <Configuration.h>
#include <IO/WebServiceRegistry.h>
#include <PubSubClient.h>
#include <Common/Serializable.h>

// the maximum length of paramters we accept
class MqttCommandHandler : public IObserver, public INotifier
{
  private:
    Configuration::WifiNetworkConfiguration &wifiConfig;
    Configuration::ServerConfig &serverConfig;
    PubSubClient pubSubClient;
    WiFiClient wifiClient;
    long lastReconnectAttempt = 0;

    void getInfo(String &data)
    {
        StaticJsonBuffer<400> jsonBuffer;
        JsonObject &root = jsonBuffer.createObject();
        root["timestamp"] = millis();
        JsonObject &payload = root.createNestedObject("payload");
        payload["id"] = serverConfig.deviceId;
        payload["topic"] = serverConfig.topicIn;
        root.printTo(data);
    }

    boolean reconnect()
    {
        if (pubSubClient.connect(serverConfig.deviceId))
        {
            String inTopic = String(serverConfig.topicIn) + "/" + String(serverConfig.deviceId) + "/#";
            pubSubClient.subscribe(inTopic.c_str());

#ifdef LOGGING
            Serial.println(F("MQTT client connected"));
            Serial.print(F("Server: "));
            Serial.print(serverConfig.host);
            Serial.print(F(":"));
            Serial.println(serverConfig.port);
            Serial.print("Listening topic: ");
            Serial.println(inTopic);
#endif
            String data;
            getInfo(data);
            publishData("/init", data.c_str());
        }
        return pubSubClient.connected();
    }

    void clientLoop()
    {
        if (!pubSubClient.connected())
        {
            long now = millis();
            if (now - lastReconnectAttempt > 5000)
            {
                lastReconnectAttempt = now;
                if (reconnect())
                    lastReconnectAttempt = 0;
            }
        }
        else
        {
            pubSubClient.loop();
        }
    }

    void initializeWiFi()
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
        Serial.print(F("Device is at "));
        Serial.print(F("mqtt://"));
        Serial.print(WiFi.localIP());
        Serial.print(F(":"));
        Serial.println(wifiConfig.port);
#endif
    }

    void initializeMqttClient()
    {
        pubSubClient = PubSubClient(serverConfig.host, serverConfig.port, wifiClient);
        pubSubClient.setCallback([this](char *topic, byte *payload, unsigned int length) {
            WebServiceRegistry::getInstance().getService(&strrchr(topic, '/')[1])->processMessage(topic, payload, length);
        });
    }

  public:
    MqttCommandHandler(Configuration::WifiNetworkConfiguration &wifiConfig_, Configuration::ServerConfig &serverConfig_)
        : wifiConfig(wifiConfig_), serverConfig(serverConfig_)
    {
    }

    ~MqttCommandHandler()
    {
    }

    void init()
    {
        initializeWiFi();
        initializeMqttClient();
        TickNotifier::getInstance().attach(this);
    }

    void update(const char *command)
    {
        if (strcmp(command, TickNotifier::getInstance().tickCommand) == 0)
        {
            clientLoop();
        }
    }

    /*virtual*/ void publishData(const char *topic, Serializable &data)
    {
        StaticJsonBuffer<400> jsonBuffer;
        JsonObject &root = jsonBuffer.createObject();
        data.toJson(root);
        String client;
        root.printTo(client);
        publishData(topic, client.c_str());
    }

    /*virtual*/ void publishData(const char *topic, const char *message)
    {
        char newTopic[100];
        strcpy(newTopic, serverConfig.topicOut);
        strcat(newTopic, "/");
        strcat(newTopic, serverConfig.deviceId);
        strcat(newTopic, topic);
#ifdef LOGGING
        Serial.print(F("@"));
        Serial.println(newTopic);
#endif
        pubSubClient.publish(&newTopic[0], message);
    }
};

#endif