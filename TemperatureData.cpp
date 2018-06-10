#include "TemperatureData.h"

#define DHTTYPE DHT11 // DHT 11

TemperatureData::TemperatureData(uint8_t sensorPin)
    :dhtSensor(DHT(sensorPin, DHT11))
{
    dhtSensor.begin();
}

TemperatureData::~TemperatureData() { }

void TemperatureData::updateData()
{
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    int hum = dhtSensor.readHumidity();
    // Read temperature as Celsius (the default)
    int temp = dhtSensor.readTemperature();
    // Check if any reads failed and exit early (to try again).
    if (isnan(temp) || isnan(hum))
        return;

    humidity = hum;
    temperature = temp;
}

void TemperatureData::toJson(JsonObject &root)
{
    root["temp"] = getTemperature();
    root["hum"] = getHumidity();
    root["hIndex"] = getHeatIndex();;
}