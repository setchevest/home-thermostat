#ifndef Thermostat_h
#define Thermostat_h

#include <ControlStrategy.h>
#include <HeaterController.h>
#include <ZoneList.h>
#include <Common/ContextableThread.h>
#include <Common/Callback.h>
#include <Common/Serializable.h>
#include <Common/Environment.h>
#include <Common/Interfaces/IObserver.h>

struct ZoneConfig
{
    int id;
    String name;
    uint8_t sensorPin;
};

struct ThermostatConfig
{
    uint8_t heaterPin;
    uint8_t heaterStatusPin;
    size_t zonesQuantity;
    ZoneConfig *zones;
};

class Thermostat : public Serializable
{
  private:
    HeaterController *heater = nullptr;
    ZoneList *zoneList = nullptr;
    ContextableThread *thread1 = nullptr;
    ThermostatConfig *config = nullptr;

    void init();
    void getDataFromSensors();
    void printTo(Print &printer);

  public:
    Thermostat(ThermostatConfig &config);
    ~Thermostat();
    void addZone(ZoneConfig &zoneConfig);
    String getFriendlyName();
    void toJson(JsonObject &root);
    void toggleHeater();
};

#endif