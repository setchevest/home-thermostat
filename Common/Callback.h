#ifndef Callback_h
#define Callback_h

#include <Arduino.h>

/*
struct X {
  void y() { std::cout << "y\n"; }
  static void z() { std::cout << "z\n"; }
} x;

void w() { std::cout << "w\n"; }

int main(int, char*[]) {
   Callback c1(x, &X::y);
   Callback c2(X::z);
   Callback c3(w);
   c1();
   c2();
   c3();
   return 0;
}
*/

class Callable
{
  public:
    virtual void operator()() = 0;
    virtual ~Callable() {}
};

// wraps pointer-to-members
template <class C>
class CallableFromObject : public Callable
{
  private:
    C &o;
    void (C::*m)();

  public:
    CallableFromObject(C &object, void (C::*method)())
        : o(object), m(method) {}

    void operator()()
    {
        (&o->*m)();
    }
};

template <typename Lambda>
class CallableFromLambda : public Callable
{
  private:
    Lambda &&l;

  public:
    CallableFromLambda(Lambda&& lambda) 
    : l(lambda) {}

    void operator()()
    {
        l();
    }
};

class NotCallable : public Callable
{
    public:
    NotCallable(){}
    void operator()()
    {
        Serial.println("Calling a Not Callable object. Please review your code");
        //Do Nothing.
    }
};

// wraps pointer-to-functions or pointer-to-static-members
class CallableFromFunction : public Callable
{
private:
    void (*f)();
  public:
    CallableFromFunction(void (*function)())
        : f(function) {}

    void operator()()
    {
        f();
    };
};

// generic wrapper for any callable
// this is the only class which is exposed to the user
class Callback : public Callable
{
  private:
    Callable &c;

  public:
    Callback() : c(*new NotCallable()) {}
    template <class C>
    Callback(C &object, void (C::*method)()) : c(*new CallableFromObject<C>(object, method)) {}
    template<typename Lambda>
    Callback(Lambda&& lambda) : c(*new CallableFromLambda<Lambda>(lambda)) { }
    explicit Callback(void (*function)()) : c(*new CallableFromFunction(function)) {}
    void operator()()
    {
        c();
    }
    ~Callback()
    {
        delete &c;
    }
};
#endif