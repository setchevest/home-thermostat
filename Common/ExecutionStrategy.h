#ifndef ExecutionStrategy_h
#define ExecutionStrategy_h

#include <Arduino.h>
#include <Common/Callback.h>

class ExecutionStrategy
{
public:
  ExecutionStrategy(Callback &callback) : callback_(callback) void start()
  {
    Serial.println(F("Starting Execution"));
    execute();
    Serial.println(F("Execution Finished"));
  }

protected:
  Callback &callback_ private : virtual void execute() = 0;
};

class OneTimeExecutionStrategy : public ExecutionStrategy
{
public:
  OneTimeExecutionStrategy(Callback &callback) : ExecutionStrategy(callback) {}

private:
  bool runned = false;
  /* virtual */ void execute()
  {
    if (!runned)
    {
      callback_();
      //Avoid second Execution.
      runned = true;
    }
  }
};

class TimerExecutionStrategy : public ExecutionStrategy
{
public:
  TimerExecutionStrategy(Callback &callback, unsigned long interval) : ExecutionStrategy(callback)
  {
    timer = new Timer(*new Callback(*this, &TimerExecutionStrategy::execute), interval);
  }

  ~TimerExecutionStrategy()
  {
    delete timer;
  }

private:
  Timer *timer = nullptr;
  /* virtual */ void execute()
  {
    callback_();
  }
};

class ManualExecutionStrategy : public ExecutionStrategy
{
public:
  ManualExecutionStrategy(Callback &callback) : ExecutionStrategy(callback) {}

private:
  /* virtual */ void execute()
  {
  }
};

#endif