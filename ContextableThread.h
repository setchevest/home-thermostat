#ifndef ContextableThread_h
#define ContextableThread_h

#include <Thread.h>
#include <Callback.h>

class ContextableThread : public Thread
{
  protected:
      Callback *_function;

  public:
    ContextableThread(Callback *callback, unsigned long interval)
     : Thread(nullptr, interval)
    {
        onRun(callback);
    }

    ~ContextableThread()
    {
         delete _function;
    }
    
    /* virtual override */ void run()
    {
         if (_function){
             (*_function)();
         }
             
        //calling here base class to set Runned = true
        Thread::run();
    }

    void onRun(Callback *callback)
    {
         _function = callback;
        Thread::onRun(nullptr);
    }

    //adding a basic check to avoid the "if" sentence in business code.
    void check()
    {
        if (shouldRun())
            run();
    }
};

#endif