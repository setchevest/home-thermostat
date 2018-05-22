#ifndef Thermostat_h
#define Thermostat_h

#include <ControlStrategy.h>
#include <HeaterController.h>
#include <ZoneList.h>
#include <Common/ContextableThread.h>
#include <Common/Callback.h>
#include <Common/Serializable.h>
#include <Common/Environment.h>
#include <IO/CommandHandler.h>

class Thermostat : public Serializable
{
  private:
    HeaterController heater;
    ZoneList zoneList;

  public:
    Thermostat(uint8_t heaterRelayPin, uint8_t heaterStatusPin);
    ~Thermostat();
    void addZone(TemperatureZone *zone);
    void addZone(int id, String name, uint8_t sensorPin);
    void keepAlive();
    String getFriendlyName();
    void toJson(JsonObject &root);
    void toggleHeater();
};

#endif

Thermostat::Thermostat(uint8_t heaterRelayPin, uint8_t heaterStatusPin)
    :heater(HeaterController(heaterRelayPin, heaterStatusPin)), zoneList(ZoneList())
{
    
}
Thermostat::~Thermostat()
{
}

void Thermostat::keepAlive()
{
    for (size_t i = 0; i < zoneList.getLenght(); i++)
    {
        zoneList[i]->updateStatus();
    }
}

void Thermostat::addZone(int id, String name, uint8_t sensorPin)
{
    zoneList.add(id, name, sensorPin);
}

String Thermostat::getFriendlyName()
{
    return "Thermostat";
}

void Thermostat::toJson(JsonObject &root)
{
    heater.toJson(root.createNestedObject("heater"));
    JsonArray &jsonZones = root.createNestedArray("zones");
    for (size_t i = 0; i < zoneList.getLenght(); i++)
    {
        zoneList[i]->toJson(jsonZones.createNestedObject());
    }
}

void Thermostat::toggleHeater()
{
    heater.toggle();
}