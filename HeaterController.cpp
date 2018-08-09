#include "HeaterController.h"

using namespace Configuration;

HeaterController::HeaterController(ThermostatConfig &_config)
    : config(_config)
{

    pinMode(config.heaterPin, OUTPUT);
#ifdef LOGGING
    Serial.print(F("Setting PIN "));
    Serial.print(config.heaterPin);
    Serial.print(F(" to "));
    Serial.println(OUTPUT);
#endif
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
#ifdef LOGGING
    Serial.print(F("Status PIN "));
    Serial.print(config.heaterPin);
    Serial.print(F(" to "));
    Serial.println(HIGH);
#endif
}

void HeaterController::off()
{
    setStatus(LOW);
#ifdef LOGGING
    Serial.println(F("Status PIN "));
    Serial.print(config.heaterPin);
    Serial.print(F(" to "));
    Serial.print(LOW);
#endif
}

void HeaterController::toggle()
{
    setStatus(getStatus() ? LOW : HIGH);
}

bool HeaterController::getStatus()
{
    return status == HIGH;
}

void HeaterController::toJson(JsonObject &root)
{
    root["status"] = getStatus() ? "ON" : "OFF";
}