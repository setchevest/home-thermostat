#include <Thermostat.h>

using namespace Configuration;

Thermostat::Thermostat(RequestConfig &server_)
    : server(server_), zoneList(new ZoneList())
{
}

Thermostat::~Thermostat()
{
    cleanup();
    delete zoneList;
}

void Thermostat::cleanup()
{
    delete heater;
    zoneList->empty();
    delete timer;
}

void Thermostat::init(ThermostatConfig _config)
{
    config = _config;
#ifdef LOGGING
    Serial.println(F("Initializing Thermostat"));
#endif
    cleanup();
    timer = new Timer(*new Callback<void>(*this, &Thermostat::check), config.updateFrequency * 1000);
    heater = new HeaterController(config);
    for (size_t i = 0; i < config.zonesQuantity; i++)
    {
        zoneList->add(new TemperatureZone(config.zones[i].id, config.zones[i].sensorPin));
        if (config.zones[i].hasControl)
            controlZoneId = config.zones[i].id;
    }
    check();
}
void Thermostat::check()
{
    getDataFromSensors();
    if (!manualModeEnabled)
        invalidateHeaterStatus();
}

void Thermostat::getDataFromSensors()
{
#ifdef LOGGING
    Serial.println(F("Getting data from Sensors"));
#endif
    if (zoneList)
    {
        for (size_t i = 0; i < zoneList->size(); i++)
        {
            if (zoneList->get(i)->updateStatus())
            {
                //Let the server know about this change (out of memory :()
            }
        }
    }
}

void Thermostat::toJson(JsonObject &root)
{
    root["fm"] = Environment::getFreeMemory();
    root["lu"] = (Environment::getNowInMilliseconds() - timer->getLastRunInMilliseconds()) / 1000;
    root["mode"] = manualModeEnabled ? "Manual" : "Automatic";
    heater->toJson(root.createNestedObject("heater"));
    JsonArray &jsonZones = root.createNestedArray("zones");
    for (size_t i = 0; i < zoneList->size(); i++)
        zoneList->get(i)->toJson(jsonZones.createNestedObject());
}

void Thermostat::invalidateHeaterStatus()
{
    for (size_t i = 0; i < zoneList->size(); i++)
    {
        if (controlZoneId == zoneList->get(i)->getId())
        {
            if (!zoneList->get(i)->isTemperatureComfortable(config.threshold.min, config.threshold.max, heater->getStatus()))
                toggleHeater();

            break;
        }
    }
}

void Thermostat::toggleHeater()
{
    if (heater)
        heater->toggle();
}

void Thermostat::heaterOn()
{
    if (heater)
        heater->on();
}

void Thermostat::heaterOff()
{
    if (heater)
        heater->off();
}

void Thermostat::setManualMode(bool manualModeEnabled_)
{
    manualModeEnabled = manualModeEnabled_;
}