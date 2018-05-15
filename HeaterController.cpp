#include "Arduino.h"
#include "HeaterController.h"

HeaterController::HeaterController(uint8_t pin)
    : HeaterController(pin, 0)
{
}

HeaterController::HeaterController(uint8_t pin, uint8_t statusLedPin)
{
    pinMode(pin, OUTPUT);
    _pin = pin;
    _statusLedpin = statusLedPin;
    if (_statusLedpin > 0)
        pinMode(_statusLedpin, OUTPUT);
}
// privates

void HeaterController::setStatus(byte newStatus)
{
    if(newStatus == HIGH || newStatus == LOW)
    {
        status = newStatus;
        digitalWrite(_pin, status);
        if (_statusLedpin > 0)
            digitalWrite(_statusLedpin, status);
    }   
}
// public
void HeaterController::on()
{
    setStatus(HIGH);
}

void HeaterController::off()
{
    setStatus(LOW);
}

void HeaterController::toggle()
{
    if(getStatus())
        setStatus(LOW);
    else
        setStatus(HIGH);
}

boolean HeaterController::getStatus()
{
    return status == HIGH;
}

String HeaterController::getFriendlyName()
{
    return "HeaterController";
}

void HeaterController::toJson(JsonObject &root)
{
    root["status"] = getStatus();
}