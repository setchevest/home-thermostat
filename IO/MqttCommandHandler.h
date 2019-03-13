#define LOGGING
#ifndef EthernetClient_h
#define EthernetClient_h

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <Common/StringPrint.h>
#include <Common/Serializable.h>
#include <Common/Interfaces/IObserver.h>
#include <Common/Interfaces/INotifier.h>
#include <Configuration.h>
#include <IO/WebServiceRegistry.h>
#include <IO/MqttCommandParser.h>
#include <PubSubClient.h>
#include <Common/Serializable.h>

// the maximum length of paramters we accept
class MqttCommandHandler : public IObserver, public INotifier
{
  private:
    Configuration::WifiNetworkConfiguration &wifiConfig;
    Configuration::ServerConfig &serverConfig;
    MqttCommandParser &parser;
    PubSubClient pubSubClient;
    WiFiClient wifiClient;
    long lastReconnectAttempt = 0;

    boolean reconnect()
    {
        if (pubSubClient.connect(wifiConfig.deviceName, serverConfig.deviceId, serverConfig.devicePass))
        {
            String inTopic = String(serverConfig.topicRpcIn) + "/+";
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
        }
        return pubSubClient.connected();
    }

    void clientLoop()
    {
        if (!pubSubClient.connected())
        {
            if (WiFi.status() != WL_CONNECTED)
            {
                connectWiFi();
            }
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

    void connectWiFi()
    {
        // WiFi.hostname(wifiConfig.deviceName);
        // WiFi.config(IPAddress(wifiConfig.defaultIp[0], wifiConfig.defaultIp[1], wifiConfig.defaultIp[2], wifiConfig.defaultIp[3]),
        //             IPAddress(wifiConfig.gateway[0], wifiConfig.gateway[1], wifiConfig.gateway[2], wifiConfig.gateway[3]),
        //             IPAddress(wifiConfig.subnet[0], wifiConfig.subnet[1], wifiConfig.subnet[2], wifiConfig.subnet[3]));
        WiFi.begin(wifiConfig.ssid, wifiConfig.pass);

        // Wait for connection
        while (WiFi.status() != WL_CONNECTED)
        {
            delay(500);
            Serial.print(".");
        }

#ifdef LOGGING
        Serial.println();
        Serial.print(F("Device IP: "));
        Serial.println(WiFi.localIP());
#endif
    }

    void initializeMqttClient()
    {
        pubSubClient = PubSubClient(wifiClient);
        pubSubClient.setServer(serverConfig.host, serverConfig.port);
        pubSubClient.setCallback([this](const char *topic, byte *payload, unsigned int length) {
            onMessageCallback(topic, payload, length);
        });
        reconnect();
    }

    void onMessageCallback(const char *topic, byte *payload, unsigned int length)
    {
        char json[length + 1];
        strncpy(json, (char *)payload, length);
        json[length] = '\0';
#ifdef LOGGING
        Serial.println(F("Incomming"));
        Serial.print("Topic: ");
        Serial.println(topic);
        Serial.print("Message: ");
        Serial.println(json);
#endif
        HttpCommand command = parser.parse(topic, payload, length);
        ActionResponse & response = WebServiceRegistry::getInstance().getService(command)->processRequest(command);
        StringPrint data;
        response.bodyFlush(data);
        // String responseTopic = String(topic);
        // responseTopic.replace("request", "response");
        publishTelemetry( data.c_str());
    }

  protected:
    /*virtual*/ void publishData(const char *topic, Serializable &data, bool retained = false)
    {
        StaticJsonBuffer<400> jsonBuffer;
        JsonObject &root = jsonBuffer.createObject();
        data.toJson(root);
        String client;
        root.printTo(client);
        publishData(topic, client.c_str());
    }

    /*virtual*/ void publishData(const char *topic, const char *message, bool retained = false)
    {
#ifdef LOGGING
        Serial.println(F("Outgoing"));
        Serial.print(F("@"));
        Serial.println(topic);
        Serial.print(F("#"));
        Serial.println(message);
#endif
        pubSubClient.publish(topic, message, retained);
    }

  public:
    MqttCommandHandler(Configuration::WifiNetworkConfiguration &wifiConfig_, Configuration::ServerConfig &serverConfig_, MqttCommandParser &parser_)
        : wifiConfig(wifiConfig_), serverConfig(serverConfig_), parser(parser_)
    {
    }

    ~MqttCommandHandler()
    {
    }

    void init()
    {
        connectWiFi();
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

    /*virtual*/ void publishTelemetry(const char *message)
    {
        publishData(serverConfig.topicOut, message, true);
    }
    /*virtual*/ void publishTelemetry(Serializable &data)
    {
        publishData(serverConfig.topicOut, data, true);
    }
    /*virtual*/ void publishAttributes(const char *message)
    {
        publishData(serverConfig.topicAttrOut, message);
    }
    /*virtual*/ void publishAttributes(Serializable &data)
    {
        publishData(serverConfig.topicAttrOut, data);
    }
};

#endif