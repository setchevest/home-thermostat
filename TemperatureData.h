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
  unsigned long lastUpdate;
  DHT dhtSensor;

public:
  TemperatureData(uint8_t sensorPin);
  TemperatureData();
  ~TemperatureData();
  float getTemperature() { return temperature || NAN; }
  float getHumidity() { return humidity || NAN; }
  float getHeatIndex() { return heatIndex || NAN; }
  float getLastUpdate() { return lastUpdate || NAN; }
  void updateData();
  /* virtual override */ String getFriendlyName() 
  { 
    return "TemperatureData"; 
  }

  /* virtual override */ void toJson(JsonObject &root);
};

#endif
