#ifndef TemperatureZone_h
#define TemperatureZone_h

#include <Common/Serializable.h>
#include <TemperatureData.h>
#include <Common/Environment.h>

class TemperatureZone : Serializable
{
  private:
    int _id;
    String _name;
    TemperatureData data;
    bool _active = true;

  public:
    TemperatureZone(const int id, const String name, uint8_t sensorPin)
        : _id(id), _name(name), data(TemperatureData(sensorPin))
    {
    }

    TemperatureZone()
        : TemperatureZone(-1, F("NONE"), 0)
    {
    }

    ~TemperatureZone() {}

    int getId() { return _id; }

    String getName() { return _name; }

    bool isActive() { return _active; }

    void activate() { _active = true; }

    void deactivate() { _active = false; }

    void updateStatus()
    {
        if (isActive())
        {
            data.updateData();
        }
    }

    bool isTemperatureComfortable(const int min,const int max,const bool isWarming)
    {
        Serial.print(F("Memory:"));
        Serial.println(Environment::getFreeMemory());
        float current = data.getHeatIndex();
        Serial.print(F("Is Warming?: "));
        Serial.print(isWarming);
        Serial.print(F("- Min: "));
        Serial.print(min);
        Serial.print(F("- Max: "));
        Serial.print(max);
        Serial.print(F("- current: "));
        Serial.print(current);
        bool res = (!isWarming && current < min) ? false : (isWarming && current > max) ? false : true;
        Serial.print(F("- Is Confortable?: "));
        Serial.println(res);
        return res;
    }

    void toJson(JsonObject &root)
    {
        root["id"] = _id;
        root["name"] = _name;
        root["active"] = _active;
        data.toJson(root);
    }
};

#endif