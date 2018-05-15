#ifndef TemperatureZone_h
#define TemperatureZone_h

#include <Serializable.h>
#include <TemperatureData.h>

class TemperatureZone : Serializable
{
  private:
    int _id;
    String _name;
    TemperatureData *data;
    bool _active;

  public:
    TemperatureZone(int id, String name, uint8_t sensorPin)
    {
        _id = id;
        _name = name;
        data = new TemperatureData(sensorPin);
    }

    ~TemperatureZone()
    {
        delete data;
    }

    bool isActive()
    {
        return _active;
    }

    void activate()
    {
        _active = false;
    }

    void updateStatus()
    {
        data->updateData();
    }

    String getFriendlyName()
    {
        return "TemperatureZone";
    }

    void toJson(JsonObject &root)
    {
        root["name"] = _name;
        root["id"] = _id;
        root["active"] = _active;
        data->toJson(root.createNestedObject("data"));
    }
};

#endif