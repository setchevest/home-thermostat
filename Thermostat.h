#ifndef Thermostat_h
#define Thermostat_h

#include <ControlStrategy.h>
#include <HeaterController.h>
#include <TemperatureZone.h>
#include <Serializable.h>
#include <LinkedList.h>

class Thermostat : public Serializable
{
  private:
    HeaterController heater;
    LinkedList<TemperatureZone *> zones;

  public:
    Thermostat(uint8_t heaterRelayPin, uint8_t heaterStatusPin);
    ~Thermostat();
    void addZone(TemperatureZone *zone);
    void addZone(int id, String name, uint8_t sensorPin);
    void updateStatus();
    String getFriendlyName();
    void toJson(JsonObject &root);
    void toggleHeater();
};

#endif

Thermostat::Thermostat(uint8_t heaterRelayPin, uint8_t heaterStatusPin)
    :heater(HeaterController(heaterRelayPin, heaterStatusPin))
{
    zones = LinkedList<TemperatureZone *>();
}

Thermostat::~Thermostat()
{
    for (size_t i = 0; i < zones.getLenght(); i++)
    {
        delete zones[i];
    }
}

void Thermostat::updateStatus()
{
    for (size_t i = 0; i < zones.getLenght(); i++)
    {
        zones[i]->updateStatus();
    }
}

void Thermostat::addZone(int id, String name, uint8_t sensorPin)
{
    addZone(new TemperatureZone(id, name, sensorPin));
}

void Thermostat::addZone(TemperatureZone *zone)
{
    zones.add(zone);
}

String Thermostat::getFriendlyName()
{
    return "Thermostat";
}

void Thermostat::toJson(JsonObject &root)
{
    heater.toJson(root.createNestedObject("heater"));
    JsonArray &data = root.createNestedArray("zones");
    for (size_t i = 0; i < zones.getLenght(); i++)
    {
        zones[i]->toJson(data.createNestedObject());
    }
    
}

void Thermostat::toggleHeater()
{
    heater.toggle();
}