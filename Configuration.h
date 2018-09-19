#ifndef Configuration_h
#define Configuration_h
#include <Arduino.h>

namespace Configuration
{

struct ServerConfig
{
    const char* host = "192.168.0.3";
    unsigned int port = 1883;
    const char* configPath = "/api/thermostat/config";
    const char* eventPath = "";
    const char* deviceId = "HomeThermostat";
    const char* topicRpcIn = "v1/devices/me/rpc/request";
    const char* topicRpcOut = "v1/devices/me/rpc/response";
    const char* topicAttrIn = "v1/devices/me/attributes/request";
    const char* topicAttrOut = "v1/devices/me/attributes";
    const char* topicOut = "v1/devices/me/telemetry";
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