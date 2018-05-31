#ifndef Configuration_h
#define Configuration_h
#include <Arduino.h>

namespace Configuration
{
struct ZoneConfig
{
    unsigned int id;
    const char* name;
    uint8_t sensorPin;
};

struct EthernetConfiguration
{
    unsigned int port = 9000;
    byte defaultMac[6] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEE};
    unsigned int defaultIp[4] = {192, 168, 0, 177};
    const char* hostURL = "www.google.com";
    unsigned int hostPort = 80;
};

struct ThermostatTemperatureConfig
{
    uint8_t min;
    uint8_t max;
    unsigned int controlZoneId;
};

struct ThermostatConfig
{
    unsigned long updateFrequency;
    uint8_t heaterPin;
    size_t zonesQuantity;
    ZoneConfig *zones;
    ThermostatTemperatureConfig threshold;
};

} // namespace Configuration

#endif