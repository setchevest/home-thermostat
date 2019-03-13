#ifndef Configuration_h
#define Configuration_h
#include <Arduino.h>

namespace Configuration
{

struct ServerConfig
{
    const char* host = "192.168.1.100";
    unsigned int port = 1883;
    const char* configPath = "/api/thermostat/config";
    const char* eventPath = "";
    const char* deviceId = "homeassistant";
    const char* devicePass = "yupi";
    const char* topicRpcIn = "devices/thermostat/request";
    const char* topicRpcOut = "devices/thermostat/response";
    const char* topicAttrIn = "devices/thermostat/attributes/request";
    const char* topicAttrOut = "devices/thermostat/attributes/response";
    const char* topicOut = "devices/thermostat/telemetry";
};

struct ZoneConfig
{
    unsigned int id;
    uint8_t sensorPin;
    bool hasControl;
};

struct EthernetConfiguration
{
    byte defaultMac[6] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEE};
    unsigned int defaultIp[4] = {192, 168, 0, 42};
    unsigned int port = 80;
    unsigned int gateway[4] = {192, 168, 0, 1};
    unsigned int subnet[4] = {255, 255, 255, 0};
};

struct WifiNetworkConfiguration : EthernetConfiguration
{
    const char *ssid = "PipySebas";
    const char *pass = "porotoencamino";
    const char *deviceName = "Thermostat";
};

struct ThermostatTemperatureConfig
{
    uint8_t min;
    uint8_t max;
};

struct ThermostatConfig
{
    unsigned int updateFrequency;
    uint8_t heaterPin;
    size_t zonesQuantity;
    ZoneConfig *zones;
    ThermostatTemperatureConfig threshold;
};

} // namespace Configuration

#endif