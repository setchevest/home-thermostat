#include <Thermostat.h>

using namespace Configuration;

Thermostat::Thermostat(RequestConfig &server_)
    : server(server_)
{
}

Thermostat::~Thermostat()
{
    delete heater;
    delete zoneList;
    delete timer;
}

void Thermostat::init(ThermostatConfig _config)
{
    config = _config;
    Serial.println(F("Initializing Thermostat"));
    delete heater;
    delete zoneList;
    delete timer;
    zoneList = new ZoneList();
    timer = new Timer(*new Callback<void>(*this, &Thermostat::check), config.updateFrequency);
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
        Serial.println(F("Getting data from Sensors"));
        for (size_t i = 0; i < zoneList->size(); i++)
        {
            zoneList->get(i)->updateStatus();
        }
    }
}

void Thermostat::addZone(ZoneConfig &zoneConfig)
{
    if (zoneList)
    {
        zoneList->add(zoneConfig.id, zoneConfig.name, zoneConfig.sensorPin);

        if (zoneConfig.hasControl)
            controlZoneId = zoneConfig.id;
    }
}

void Thermostat::toJson(JsonObject &root)
{
    root["freeMem"] = Environment::getFreeMemory();
    root["lastUpdate"] = (Environment::getNowInMilliseconds() - timer->getLastRunInMilliseconds()) / 1000;
    heater->toJson(root.createNestedObject("heater"));
    JsonArray &jsonZones = root.createNestedArray("zones");
    for (size_t i = 0; i < zoneList->size(); i++)
        zoneList->get(i)->toJson(jsonZones.createNestedObject());
}

void Thermostat::invalidateHeaterStatus()
{
    if (zoneList)
    {
        TemperatureZone *zone = zoneList->getById(controlZoneId);
        if (zone && !zone->isTemperatureComfortable(config.threshold.min, config.threshold.max, heater->getStatus()))
            toggleHeater();
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

void Thermostat::getConfigFromServer()
{
    int err = 0;
    EthernetClient c;
    HttpClient http(c);
    Serial.print(F("Calling server: "));
    Serial.print(server.host);
    Serial.print(F(":"));
    Serial.print(server.port);
    Serial.println(server.path);
    err = http.get(server.host, server.port, server.path);
    if (err == 0)
    {
        err = http.responseStatusCode();
        if (err >= 0)
        {
            // Usually you'd check that the response code is 200 or a
            // similar "success" code (200-299) before carrying on,
            // but we'll print out whatever response we get

            err = http.skipResponseHeaders();
            if (err >= 0)
            {
                int bodyLen = http.contentLength();
                // Now we've got to the body, so we can print it out
                unsigned long timeoutStart = millis();
                char c;
                // Whilst we haven't timed out & haven't reached the end of the body
                while ((http.connected() || http.available()) &&
                       ((millis() - timeoutStart) < kNetworkTimeout))
                {
                    if (http.available())
                    {
                        c = http.read();
                        // Print out this character
                        Serial.print(c);
                        // We read something, reset the timeout counter
                        timeoutStart = millis();
                    }
                    else
                    {
                        // We haven't got any data, so let's pause to allow some to arrive
                        delay(kNetworkDelay);
                    }
                }
            }
            else
            {
                Serial.print(F("Failed to skip response headers: "));
                Serial.println(err);
            }
        }
        else
        {
            Serial.print(F("Getting response failed: "));
            Serial.println(err);
        }
    }
    else
    {
        Serial.print(F("Connect failed: "));
        Serial.println(err);
    }
    http.stop();
}