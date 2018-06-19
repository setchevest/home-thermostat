#include <Thermostat.h>
#include <Configuration.h>
#include <IO/EthernetCommandHandler.h>
#include <Common/Callback.h>
#include <IO/HttpCommandParser.h>
#include <IO/WebServiceRegistry.h>
#include <ThermostatWebService.h>

Configuration::RequestConfig server = {"192.168.0.13", 8080, "/api/thermostat/config"}; //Server Configuration

Configuration::ThermostatConfig tc = {10000 /*updateFrequency*/, 7,/*Relay pin*/
                                      1,/*zones Quantity*/
                                      new Configuration::ZoneConfig[1]{
                                          // {1,/*id*/ "Zone 1",/*name*/ 8/*pin*/},
                                      {1, /*id*/ "Zone 2", /*name*/ 9 /*pin*/, true /*hasControl*/}},
                                      {19, /*min*/ 21, /*max*/}};
//using default instance;
Configuration::EthernetConfiguration ec = {
    {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEE}, //Defaul MacAddress
    {192, 168, 0, 16},                    //Default Ip In Case DNS is not working
    80                                    //Port to listen
};
Thermostat *thermo;
EthernetCommandHandler *ethernetHandler;
HttpCommandParser parser;
void setup()
{
  //Serial.begin(115200);
  thermo = new Thermostat(server);
  ethernetHandler = new EthernetCommandHandler(ec, parser);
  WebServiceRegistry::getInstance().registerService(new ThermostatWebService(*thermo));
  ethernetHandler->init();
  thermo->init(tc);
  // initialize the pushbutton pin as an input:
  pinMode(3, INPUT);
  // Attach an interrupt to the ISR vector
  attachInterrupt(digitalPinToInterrupt(3), buttonPressed, RISING);
}

void buttonPressed()
{
  Serial.println(F("Auto mode ON"));
  thermo->setManualMode(false);
}

void loop()
{
  TickNotifier::getInstance().notify();
}
