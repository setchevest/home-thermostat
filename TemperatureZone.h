#ifndef TemperatureZone_h
#define TemperatureZone_h

#include <Common/Serializable.h>
#include <TemperatureData.h>
#include <Common/Environment.h>

class TemperatureZone : Serializable
{
  private:
    unsigned int _id;
    // const char *_name;
    TemperatureData data;

  public:
    TemperatureZone(unsigned int id, const char *name, uint8_t sensorPin)
        : _id(id), data(TemperatureData(sensorPin)) //,_name(name)
    {
        
    }

    ~TemperatureZone() {}

    unsigned int getId() { return _id; }

    // const char *getName() { return _name; }

    void updateStatus()
    {
        data.updateData();
    }

    bool isTemperatureComfortable(const int min, const int max, const bool isWarming)
    {
        float current = this->data.getHeatIndex();
        Serial.print(F("Heater: "));
        Serial.print(isWarming ? "ON" : "OFF");
        Serial.print(F("  -Min: "));
        Serial.print(min);
        Serial.print(F(" -Max: "));
        Serial.print(max);
        Serial.print(F(" -HeatIndex: "));
        Serial.print(current);
        bool res = (!isWarming && current < min) ? false : (isWarming && current > max) ? false : true;
        Serial.print(F("- Is Confortable?: "));
        Serial.println(res ? "YES" : "NO");
        return res;
    }

    void toJson(JsonObject &root)
    {
        root["id"] = _id;
        // root["name"] = _name;
        // root["active"] = _active;
        data.toJson(root);
    }
};

#endif