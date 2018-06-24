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

template <typename result>
class Callable
{
  public:
    virtual result operator()() = 0;
    virtual ~Callable() {}
};

// wraps pointer-to-members
template <class C, typename result>
class CallableFromObject : public Callable<result>
{
  private:
    C &o;
    result (C::*m)();

  public:
    CallableFromObject(C &object, result (C::*method)())
        : o(object), m(method) {}

    result operator()()
    {
        return (&o->*m)();
    }
};

template <typename Lambda, typename result>
class CallableFromLambda : public Callable<result>
{
  private:
    Lambda &&l;

  public:
    CallableFromLambda(Lambda &&lambda)
        : l(lambda) {}

    result operator()()
    {
        return l();
    }
};

template <typename result>
class NotCallable : public Callable<result>
{
  public:
    NotCallable() {}
    result operator()()
    {
#ifdef LOGGING
        Serial.println(F("Calling a Not Callable object. Please review your code"));
#endif
        return 0;
    }
};

// wraps pointer-to-functions or pointer-to-static-members
template <typename result>
class CallableFromFunction : public Callable<result>
{
  private:
    result (*f)();

  public:
    CallableFromFunction(result (*function)())
        : f(function) {}

    result operator()()
    {
        return f();
    };
};

// generic wrapper for any callable
// this is the only class which is exposed to the user
template <typename result>
class Callback : public Callable<result>
{
  private:
    Callable<result> &c;

  public:
    Callback() : c(*new NotCallable<result>()) {}
    template <class C>
    Callback(C &object, result (C::*method)()) : c(*new CallableFromObject<C, result>(object, method)) {}
    template <typename Lambda>
    Callback(Lambda &&lambda) : c(*new CallableFromLambda<Lambda, result>(lambda)) {}
    explicit Callback(result (*function)()) : c(*new CallableFromFunction<result>(function)) {}
    
    result operator()()
    {
        return c();
    }

    virtual result execute()
    {
        return c();
    }

    ~Callback()
    {
        delete &c;
    }
};
#endif