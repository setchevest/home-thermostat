#include <Thermostat.h>

using namespace Configuration;

Thermostat::Thermostat(ThermostatConfig &_config)
    : config(_config)
{
}

Thermostat::~Thermostat()
{
    delete heater;
    delete zoneList;
    delete timer;
}

void Thermostat::init()
{
    Serial.println("Initializing Thermostat");
    zoneList = new ZoneList();
    timer = new Timer(*new Callback(*this, &Thermostat::check), config.updateFrequency);
    heater = new HeaterController(config);
    for (size_t i = 0; i < config.zonesQuantity; i++)
    {
        addZone(config.zones[i]);
    }
    check();
}
void Thermostat::check()
{
    getDataFromSensors();
    invalidateHeaterStatus();
}

void Thermostat::getDataFromSensors()
{
    if (zoneList)
    {
        Serial.println("Getting data from Sensors");
        for (size_t i = 0; i < zoneList->size(); i++)
        {
            zoneList->get(i)->updateStatus();
        }
    }
}

void Thermostat::addZone(ZoneConfig &zoneConfig)
{
    if (zoneList)
        zoneList->add(zoneConfig.id, zoneConfig.name, zoneConfig.sensorPin);
}

void Thermostat::toJson(JsonObject &root)
{
    root["lastUpdate"] = timer->getLastRunInMilliseconds();
    heater->toJson(root.createNestedObject("heater"));
    JsonArray &jsonZones = root.createNestedArray("zones");
    for (size_t i = 0; i < zoneList->size(); i++)
        zoneList->get(i)->toJson(jsonZones.createNestedObject());
}

void Thermostat::invalidateHeaterStatus()
{
    if (zoneList)
    {
        TemperatureZone *zone = zoneList->getById(config.threshold.controlZoneId);        
        if (!zone->isTemperatureComfortable(config.threshold.min, config.threshold.max, heater->getStatus()))
            toggleHeater();

    }
}

void Thermostat::toggleHeater()
{
    if (heater)
        heater->toggle();
}