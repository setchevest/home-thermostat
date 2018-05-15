#ifndef TemperatureData_h
#define TemperatureData_h

#include <Arduino.h>
#include <DHT.h>
#include <Serializable.h>

class TemperatureData : Serializable
{
private:
  uint8_t sensorPin;
  float temperature;
  float humidity;
  float heatIndex;
  unsigned long lastUpdate;
  DHT *dhtSensor;

public:
  TemperatureData(uint8_t sensorPin);
  ~TemperatureData();
  float getTemperature() { return temperature || 0; }
  float getHumidity() { return humidity || 0; }
  float getHeatIndex() { return heatIndex || 0; }
  float getLastUpdate() { return lastUpdate || 0; }
  void updateData();
  /* virtual override */ String getFriendlyName() 
  { 
    return "TemperatureData"; 
  }

  /* virtual override */ void toJson(JsonObject &root);
};

#endif
