#ifndef HeaterController_h
#define HeaterController_h

#include <Arduino.h>
#include <Common/Serializable.h>
#include <Configuration.h>
using namespace Configuration;

class HeaterController : public Serializable
{
  public:
    HeaterController(ThermostatConfig &_config);
    virtual void on();
    virtual void off();
    virtual bool getStatus();
    virtual void toggle();
    virtual void toJson(JsonObject &root);

  protected:
    virtual void setStatus(byte newStatus);

  private:
    ThermostatConfig &config;
    byte status;
};

#endif
