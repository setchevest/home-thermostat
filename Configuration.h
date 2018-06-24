#ifndef Configuration_h
#define Configuration_h
#include <Arduino.h>

namespace Configuration
{

struct RequestConfig
{
    const char* host;
    unsigned int port;
    const char* configPath;
    const char* eventPath;
};

struct ZoneConfig
{
    unsigned int id;
    uint8_t sensorPin;
    bool hasControl;
};

struct EthernetConfiguration
{
    byte defaultMac[6];
    unsigned int defaultIp[4];
    unsigned int port;
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