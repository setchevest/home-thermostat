#ifndef ZoneList_h
#define ZoneList_h
#include <Common/LinkedList.h>
#include <TemperatureZone.h>
#include <Common/Interfaces/IObserver.h>

class ZoneList : public LinkedList<TemperatureZone *>
{
public:
    ~ZoneList()
    {
        for(size_t i = 0; i < size(); i++)
            delete this->get(i);
    }
    
    TemperatureZone* add(int id, String name, uint8_t sensorPin)
    {
        TemperatureZone *zone = new TemperatureZone(id, name, sensorPin);
        LinkedList::add(zone);
        return zone;
    }
};
#endif