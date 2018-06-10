#ifndef TemperatureData_h
#define TemperatureData_h

#include <Arduino.h>
#include <DHT.h>
#include <Common/Serializable.h>

class TemperatureData : Serializable
{
private:
  int temperature;
  int humidity;
  DHT dhtSensor;

public:
  TemperatureData(const uint8_t sensorPin);
  TemperatureData();
  ~TemperatureData();
  int getTemperature() { return temperature; }
  int getHumidity() { return humidity; }
  float getHeatIndex() { return dhtSensor.computeHeatIndex(getTemperature(), getHumidity(), false);; }
  void updateData();

  /* virtual override */ void toJson(JsonObject &root);
};

#endif
