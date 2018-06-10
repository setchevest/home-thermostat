#include "HeaterController.h"

using namespace Configuration;

HeaterController::HeaterController(ThermostatConfig &_config)
    :config(_config)
{
    
    pinMode(config.heaterPin, OUTPUT);
    off();
}
// privates

void HeaterController::setStatus(byte newStatus)
{
    if (newStatus == HIGH || newStatus == LOW)
    {
        status = newStatus;
        digitalWrite(config.heaterPin, status);
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
    if (getStatus())
        setStatus(LOW);
    else
        setStatus(HIGH);
}

bool HeaterController::getStatus()
{
    return status == HIGH;
}

void HeaterController::toJson(JsonObject &root)
{
    root["status"] = getStatus();
}