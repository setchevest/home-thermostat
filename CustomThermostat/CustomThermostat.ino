#include <Thermostat.h>

ThermostatConfig tc = {2/*Relay pin*/, 
                      4/*Status pin*/,
                      2/*zones Quantity*/,
                      new ZoneConfig [2] {
                        {1/*id*/, "Zone 1"/*name*/, 8/*pin*/}, 
                        {1/*id*/, "Zone 1"/*name*/, 9/*pin*/}
                      }};
Thermostat thermo = Thermostat(tc);

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  TickNotifier::getInstance().notify();
}
