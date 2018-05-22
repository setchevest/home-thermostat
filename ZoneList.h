#ifndef ZoneList_h
#define ZoneList_h
#include <Common/LinkedList.h>
#include <TemperatureZone.h>

class ZoneList : public LinkedList<TemperatureZone *>
{
public:
    ~ZoneList()
    {
        for(size_t i = 0; i < getLenght(); i++)
        {
            delete this->get(i);
        }
    }
    
    void add(int id, String name, uint8_t sensorPin)
    {
        LinkedList::add(new TemperatureZone(id, name, sensorPin));
    }
};
#endif