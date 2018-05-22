#ifndef HeaterController_h
#define HeaterController_h

#include <Arduino.h>
#include <Common/Serializable.h>

class HeaterController : public Serializable
{
  public:
    HeaterController(uint8_t pin);
    HeaterController(uint8_t pin, uint8_t statusLedPin = 0);
    virtual void on();
    virtual void off();
    virtual boolean getStatus();
    virtual void toggle();
    virtual String getFriendlyName();
    virtual void toJson(JsonObject &root);

  protected:
    virtual void setStatus(byte newStatus);

  private:
    uint8_t _pin;
    uint8_t _statusLedpin;
    byte status;
};

#endif
