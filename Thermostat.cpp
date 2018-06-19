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
    Serial.println(F("Initializing Thermostat"));
    cleanup();
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
    if (!manualModeEnabled)
        invalidateHeaterStatus();
}

void Thermostat::getDataFromSensors()
{
    Serial.println(F("Getting data from Sensors"));
    if (zoneList)
    {
        for (size_t i = 0; i < zoneList->size(); i++)
        {
            zoneList->get(i)->updateStatus();
        }
    }
}

void Thermostat::addZone(ZoneConfig &zoneConfig)
{
    zoneList->add(new TemperatureZone(zoneConfig.id, zoneConfig.name, zoneConfig.sensorPin));
    if (zoneConfig.hasControl)
        controlZoneId = zoneConfig.id;
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
            if(!zoneList->get(i)->isTemperatureComfortable(config.threshold.min, config.threshold.max, heater->getStatus()))
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