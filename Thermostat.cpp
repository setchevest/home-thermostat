#include <Thermostat.h>

Thermostat::Thermostat(ThermostatConfig &_config)
{
    config = &_config;
    init();
}

Thermostat::~Thermostat()
{
    delete config;
    delete heater;
    delete zoneList;
    delete thread1;
}

void Thermostat::init()
{
    zoneList = new ZoneList();
    thread1 = new ContextableThread(*new Callback(*this, &Thermostat::getDataFromSensors), 10000);
    heater = new HeaterController(config->heaterPin, config->heaterStatusPin);

    ZoneConfig *zc;
    for (size_t i = 0; i < config->zonesQuantity; i++)
    {
        addZone(config->zones[i]);
    }
}

void Thermostat::getDataFromSensors()
{
    for (size_t i = 0; i < zoneList->size(); i++)
    {
        zoneList->get(i)->updateStatus();
    }

    printTo(Serial);
    Serial.println();
}

void Thermostat::addZone(ZoneConfig &zoneConfig)
{
    zoneList->add(zoneConfig.id, zoneConfig.name, zoneConfig.sensorPin);
}

String Thermostat::getFriendlyName()
{
    return "Thermostat";
}

void Thermostat::toJson(JsonObject &root)
{
    root["heaterPin"] = config->heaterPin;
    root["heaterStatusPin"] = config->heaterStatusPin;
    heater->toJson(root.createNestedObject("heater"));
    JsonArray &jsonZones = root.createNestedArray("zones");
    for (size_t i = 0; i < zoneList->size(); i++)
        zoneList->get(i)->toJson(jsonZones.createNestedObject());
}

void Thermostat::toggleHeater()
{
    if (heater != nullptr)
        heater->toggle();
}

void Thermostat::printTo(Print &printer)
{
    DynamicJsonBuffer jsonBuffer(500);
    JsonObject &root = jsonBuffer.createObject();
    toJson(root);
    root.printTo(printer);
    //root.prettyPrintTo(printer);
}