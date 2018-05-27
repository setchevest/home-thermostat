#ifndef Configuration_h
#define Configuration_h
#include <Arduino.h>
#include <Ethernet.h>

namespace Configuration
{
struct ZoneConfig
{
    int id;
    String name;
    uint8_t sensorPin;
};

struct EthernetConfiguration
{
    int port = 9000;
    byte defaultMac[6] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEE};
    IPAddress defaultIp = IPAddress(192, 168, 0, 177);
};

struct ThermostatTemperatureConfig
{
    uint8_t min;
    uint8_t max;
    int controlZoneId;
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