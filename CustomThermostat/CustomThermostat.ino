#include <Thermostat.h>
#include <Configuration.h>
#include <IO/EthernetCommandHandler.h>

Configuration::ThermostatConfig tc = {60000,/*updateFrequency*/
                      7,/*Relay pin*/ 
                      1,/*zones Quantity*/
                      new Configuration::ZoneConfig [1] {
                        // {1,/*id*/ "Zone 1",/*name*/ 8/*pin*/}, 
                        {2,/*id*/ "Zone 2",/*name*/ 9/*pin*/}
                      },
                      {
                        20, /*min*/
                        23, /*max*/
                        2 /*Zone*/
                      }};
//using default instance;
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

  // initialize the pushbutton pin as an input:
  pinMode(3, INPUT);
  // Attach an interrupt to the ISR vector
  attachInterrupt(digitalPinToInterrupt(3), buttonPressed, RISING);
}

void buttonPressed()
{
  ethernetHandler->callServerAsync();
}

void loop()
{
  TickNotifier::getInstance().notify();
}
