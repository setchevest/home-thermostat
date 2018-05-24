#ifndef IObserver_h
#define IObserver_h

#include <Common/LinkedList.h>

class IObserver
{
public:
  virtual void update() = 0;
};

class IObservable
{
public:
  virtual void attach(IObserver *obs) = 0;
};

class GenericObserver : IObserver
{
private:
  IObservable &model;

public:
  GenericObserver(IObservable &mod)
      : model(mod)
  {
    model.attach(this);
  }

protected:
  IObservable *getObservable()
  {
    return &model;
  }
};

class GenericObservable : IObservable
{
private:
  IList<IObserver *> *observers;

public:
  GenericObservable()
      : observers(new LinkedList<IObserver *>())
  {
  }

  ~GenericObservable()
  {
    for (int i = 0; i < observers->size(); i++)
      delete observers->get(i);

    delete observers;
  }

  void attach(IObserver *obs)
  {
    observers->add(obs);
  }

  void notify()
  {
    for (int i = 0; i < observers->size(); i++)
      observers->get(i)->update();
  }
};

#endif
