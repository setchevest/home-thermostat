#ifndef TemperatureZone_h
#define TemperatureZone_h

#include <Common/Serializable.h>
#include <TemperatureData.h>

class TemperatureZone : Serializable
{
  private:
    unsigned int _id;
    // const char *_name;
    TemperatureData data;

  public:
    TemperatureZone(unsigned int id, uint8_t sensorPin)
        : _id(id), data(TemperatureData(sensorPin)) //,_name(name)
    {
    }

    ~TemperatureZone() {}

    unsigned int getId() { return _id; }

    // const char *getName() { return _name; }

    bool updateStatus()
    {
        return data.updateData();
    }

    bool isTemperatureComfortable(const int min, const int max, const bool isWarming)
    {
        int current = this->data.getHeatIndex();
        bool res = (!isWarming && current <= min) ? false : (isWarming && current >= max) ? false : true;
#ifdef LOGGING
        Serial.print("Heater: ");
        Serial.print(isWarming ? "ON" : "OFF");
        Serial.print("  -Min: ");
        Serial.print(min);
        Serial.print(" -Max: ");
        Serial.print(max);
        Serial.print(" -HeatIndex: ");
        Serial.print(current);
        Serial.print(" -Temp: ");
        Serial.print(this->data.getTemperature());
        Serial.print(" -Hum: ");
        Serial.print(this->data.getHumidity());
        Serial.print("- Is Confortable?: ");
        Serial.println(res ? "YES" : "NO");
#endif
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