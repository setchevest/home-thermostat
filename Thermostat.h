#ifndef Thermostat_h
#define Thermostat_h

#include <Configuration.h>
#include <HeaterController.h>
#include <ZoneList.h>
#include <Common/Timer.h>
#include <Common/Callback.h>
#include <Common/Serializable.h>
#include <Common/Environment.h>
#include <Common/Interfaces/IObserver.h>

using namespace Configuration;

class Thermostat : public Serializable
{
  private:
    HeaterController *heater = nullptr;
    ZoneList *zoneList = nullptr;
    Timer *timer = nullptr;
    ThermostatConfig &config;
    void getDataFromSensors();
    void check();

  public:
    Thermostat(ThermostatConfig &config);
    ~Thermostat();
    void init();
    void addZone(ZoneConfig &zoneConfig);
    void toJson(JsonObject &root);
    void invalidateHeaterStatus();
    void toggleHeater();
};

#endif