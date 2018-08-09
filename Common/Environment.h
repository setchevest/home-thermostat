#ifndef Environment_h
#define Environment_h

#include <Common/interfaces/IObserver.h>
#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <Arduino.h>

#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char *sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif // __arm__

#endif

namespace Environment
{

#ifdef ESP8266
static unsigned int getFreeMemory()
{
    return ESP.getFreeHeap();
}
#else
static unsigned int getFreeMemory()
{
    char top;
#ifdef __arm__
    return &top - reinterpret_cast<char *>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
    return &top - __brkval;
#else  // __arm__
    return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif // __arm__
}
#endif // ESP
static void printDiagnosticData(Print &p)
{
#ifdef LOGGING
    p.print(F("Free Memory: "));
    p.println(getFreeMemory());
    delay(1);
#endif
}

static unsigned long getNowInMilliseconds()
{
    return millis();
}

} // namespace Environment

class TickNotifier : public GenericObservable
{
  public:
    const char *tickCommand = "Tick";
    static TickNotifier &getInstance()
    {
        static TickNotifier instance; // Guaranteed to be destroyed.
        return instance;              // Instantiated on first use.
    }
    // Don't forget to declare these two. You want to make sure they
    // are unacceptable otherwise you may accidentally get copies of
    // your singleton appearing.
    TickNotifier(TickNotifier const &) = delete;
    void operator=(TickNotifier const &) = delete;

    /*override*/ void notify()
    {
        GenericObservable::notify(tickCommand);
    }

  private:
    TickNotifier() {} // Constructor? (the {} brackets) are needed here.
};
#endif