#ifndef TemperatureZone_h
#define TemperatureZone_h

#include <Common/Serializable.h>
#include <TemperatureData.h>

class TemperatureZone : Serializable
{
  private:
    int _id;
    uint8_t _sensorPin;
    String _name;
    TemperatureData data;
    bool _active = true;

  public:
    TemperatureZone(const int id, const String name, uint8_t sensorPin)
        : _id(id), _name(name), data(TemperatureData(sensorPin)), _sensorPin(sensorPin)
    {}

    TemperatureZone()
        : TemperatureZone(-1, F("NONE"), 0)
    {}

    ~TemperatureZone()
    {}

    bool isActive()
    {
        return _active;
    }

    void activate()
    {
        _active = true;
    }

    void deactivate()
    {
        _active = false;
    }

    void updateStatus()
    {
        if (isActive())
        {
            data.updateData();
        }
    }

    String getFriendlyName()
    {
        return "TemperatureZone";
    }

    void toJson(JsonObject &root)
    {
        root["sensorPin"] = _sensorPin;
        root["id"] = _id;
        root["name"] = _name;
        root["active"] = _active;
        data.toJson(root.createNestedObject("data"));
    }
};

#endif