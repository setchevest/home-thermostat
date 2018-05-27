#include <Thermostat.h>
#include <Configuration.h>
#include <IO/EthernetCommandHandler.h>

Configuration::ThermostatConfig tc = {60000,/*updateFrequency*/
                      2,/*Relay pin*/ 
                      2,/*zones Quantity*/
                      new Configuration::ZoneConfig [2] {
                        {1,/*id*/ "Zone 1",/*name*/ 8/*pin*/}, 
                        {2,/*id*/ "Zone 2",/*name*/ 9/*pin*/}
                      },
                      {
                        20, /*min*/
                        23, /*max*/
                        1 /*Zone*/
                      }};
Configuration::EthernetConfiguration ec;
Thermostat *thermo;
EthernetCommandHandler *ethernetHandler;
void setup()
{ 
  Serial.begin(115200);
  thermo = new Thermostat(tc);
  thermo->init();
  ethernetHandler = new EthernetCommandHandler(ec, *thermo);
  ethernetHandler->init();
}

void loop()
{
  TickNotifier::getInstance().notify();
}
