#define LOGGING true;
#define ESP8266
#include <Thermostat.h>
#include <Configuration.h>
#include <IO/Esp8266CommandHandler.h>
#include <IO/WebServiceRegistry.h>
#include <ThermostatWebService.h>


Configuration::RequestConfig server = {"192.168.0.15", 8080, "/api/thermostat/config", ""}; //Server Configuration

Configuration::ThermostatConfig tc = {15 /*updateFrequency*/, 5, /*Relay pin*/
                                      1,                         /*zones Quantity*/
                                      new Configuration::ZoneConfig[1]{{1 /*id*/,  14 /*pin*/, true /*hasControl*/}},
                                      {19, /*min*/ 21, /*max*/}};
//using default instance;
Configuration::EthernetConfiguration ec = {
    {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEE}, //Defaul MacAddress
    {192, 168, 0, 4},                     //Default Ip In Case DNS is not working
    80,                                    //Port to listen
    {192, 168, 0, 1},                     //Gateway
    {255, 255, 255, 0},                   //Subnet
};

Configuration::WifiNetworkConfiguration wc = {
    "PipySebas", //SSID
    "porotoencamino", //Pass
    "Thermostat"
};

Thermostat *thermo;
Esp8266CommandHandler *esp8266handler;
void setup()
{
#ifdef LOGGING
  Serial.begin(115200);
#endif
	thermo = new Thermostat(server);
	esp8266handler = new Esp8266CommandHandler(ec, wc);
	WebServiceRegistry::getInstance().registerService(new ThermostatWebService(*thermo));
	esp8266handler->init();
  thermo->init(tc);
}

void loop()
{
  TickNotifier::getInstance().notify();
}