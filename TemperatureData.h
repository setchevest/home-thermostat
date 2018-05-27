#ifndef TemperatureData_h
#define TemperatureData_h

#include <Arduino.h>
#include <DHT.h>
#include <Common/Serializable.h>

class TemperatureData : Serializable
{
private:
  uint8_t sensorPin;
  float temperature;
  float humidity;
  float heatIndex;
  DHT dhtSensor;

public:
  TemperatureData(const uint8_t sensorPin);
  TemperatureData();
  ~TemperatureData();
  float getTemperature() { return temperature; }
  float getHumidity() { return humidity; }
  float getHeatIndex() { return heatIndex; }
  void updateData();

  /* virtual override */ void toJson(JsonObject &root);
};

#endif
