#include <Thermostat.h>
#define LOGGING
using namespace Configuration;

Thermostat::Thermostat(INotifier &notifier_)
    :notifier(notifier_), zoneList(new ZoneList())
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
    notifier.publishAttributes(*this);
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
                notifyChange(*zoneList->get(i));
            }
        }
    }
}

void Thermostat::notifyChange(Serializable & data)
{
    notifier.publishTelemetry(data);
}

void Thermostat::toJson(JsonObject &root)
{
    root["fm"] = Environment::getFreeMemory();
    root["lu"] = (Environment::getNowInMilliseconds() - timer->getLastRunInMilliseconds()) / 1000;
    root["mode"] = manualModeEnabled ? "M" : "A";
    heater->toJson(root);
    for (size_t i = 0; i < zoneList->size(); i++)
        zoneList->get(i)->toJson(root);
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
    {
        heater->toggle();
        notifyChange(*heater);
    }
        
}

void Thermostat::heaterOn()
{
    if (heater)
    {
        heater->on();
        notifyChange(*heater);
    }
        
}

void Thermostat::heaterOff()
{
    if (heater)
    {
        heater->off();
        notifyChange(*heater);
    }
        
}

void Thermostat::setManualMode(bool manualModeEnabled_)
{
    manualModeEnabled = manualModeEnabled_;
    notifyChange(*this);
}

void Thermostat::toggleMode()
{
    setManualMode(!manualModeEnabled);
}