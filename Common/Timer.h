#ifndef Timer_h
#define Timer_h

#include <Common/Callback.h>
#include <Common/Environment.h>
#include <Common/Interfaces/IObserver.h>

class Timer : public IObserver
{
  private:
    unsigned long lastRun;  // last time timer runned in milliseconds
    unsigned long interval; // delay between updates, in milliseconds
  protected:
    Callback &_function;

  public:
    Timer(Callback &callback, unsigned long _interval)
        : _function(callback), interval(_interval), lastRun(Environment::getNowInMilliseconds())

    {
        TickNotifier::getInstance().attach(this);
    }

    ~Timer()
    {
        delete &_function;
    }

    unsigned long getLastRunInMilliseconds()
    {
        return lastRun;
    }

    void run()
    {
        _function();

        lastRun = Environment::getNowInMilliseconds();
    }

    bool shouldRun()
    {
        return millis() - lastRun > interval;
    }

    //adding a basic check to avoid the "if" sentence in business code.
    void update(const char* command)
    {
        if (shouldRun())
            run();
    }
};

#endif