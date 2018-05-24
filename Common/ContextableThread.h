#ifndef ContextableThread_h
#define ContextableThread_h

#include <Thread.h>
#include <Common/Callback.h>
#include <Common/Environment.h>
#include <Common/Interfaces/IObserver.h>


class ContextableThread : public Thread, public IObserver
{
  protected:
    Callback &_function;

  public:
    ContextableThread(Callback &callback, unsigned long interval)
        : Thread(nullptr, interval), _function(callback)

    {
        TickNotifier::getInstance().attach(this);
    }

    ~ContextableThread()
    {
    }

    /* virtual override */ void run()
    {
        _function();
        //calling here base class to set Runned = true
        Thread::run();
    }

    //adding a basic check to avoid the "if" sentence in business code.
    void update()
    {
        if (shouldRun())
            run();
    }
};

#endif