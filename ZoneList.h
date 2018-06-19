#ifndef ZoneList_h
#define ZoneList_h
#include <Common/Interfaces/IList.h>
#include <TemperatureZone.h>

class ZoneList : public IList<TemperatureZone *>
{
    private:
     TemperatureZone *zone;
  public:
    ~ZoneList()
    {
        empty();
    }

    size_t size()
    {
        return 1;
    }

    TemperatureZone* get(int index)
    {
        return zone;
    }

    void add(TemperatureZone *data)
    {
        zone = data;
    }

    void empty()
    {
        for (size_t i = 0; i < size(); i++)
            delete this->get(i);
    }

    TemperatureZone * operator[](int index)
    {
        return zone;
    }
};
#endif