#ifndef NamedCallbackRegistry_h
#define NamedCallbackRegistry_h
#include <Common/LinkedList.h>
#include <Common/Callback.h>

class NamedCallbackRegistry : public LinkedList<NamedCallback*>
{
public:
    NamedCallback* getByName(const char* name)
    {
        NamedCallback* result = nullptr;
        for (size_t i = 0; i < size(); i++)
        {
            result = get(i);
            if (result->match(name))
                break;
            
            result = nullptr;
        }
        return result;
    }

    bool handle(const char* name)
    {
        NamedCallback* command = getByName(name);
        if(command!= nullptr)
        {
            (*command)();
            return true;
        }
        return false;
    }

    NamedCallback* operator[](const char* name) {
		return getByName(name);
	}
};

#endif