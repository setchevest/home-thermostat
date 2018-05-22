#ifndef Command_h
#define Command_h
#include <Common/Callback.h>

class CallbackCommand
{
  private:
    const char *key;
    const char *description;
    Callback &callback;

  public:
    CallbackCommand(const char *_key, const char *_description, Callback &_callback)
        : key(_key), description(_description), callback(_callback)
    {
    }
    CallbackCommand(const char *_key, Callback &_callback)
        : CallbackCommand(_key, "", _callback)
    {
    }

    ~CallbackCommand() {}

    bool match(const char *aKey)
    {
        return strcmp(aKey, key) == 0;
    }

    bool runIfMatch(const char *aKey)
    {
        if (match(aKey))
        {
            callback();
            return true;
        }
        return false;
    }
};

#endif