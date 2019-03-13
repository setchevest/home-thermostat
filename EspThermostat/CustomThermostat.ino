#define LOGGING true
#define ESP8266 true
#include <Thermostat.h>
#include <Configuration.h>
#include <IO/MqttCommandHandler.h>
#include <IO/MqttCommandParser.h>
#include <IO/WebServiceRegistry.h>
#include <ThermostatWebService.h>
#include <PubSubClient.h>

Configuration::ServerConfig serverConfig; //Server Configuration

Configuration::ThermostatConfig thermoConfig = {15 /*updateFrequency*/, 5, /*Relay pin*/
                                      1,                         /*zones Quantity*/
                                      new Configuration::ZoneConfig[1]{{2 /*id*/, 4 /*pin*/, true /*hasControl*/}},
                                      {19, /*min*/ 21, /*max*/}};
//using default instance;
Configuration::WifiNetworkConfiguration netConfig;

Thermostat *thermo = nullptr;
MqttCommandHandler *handler = nullptr;
MqttCommandParser parser;

void setup()
{
#ifdef LOGGING
  Serial.begin(115200);
#endif
  handler = new MqttCommandHandler(netConfig, serverConfig, parser);
  thermo = new Thermostat(*handler);
  WebServiceRegistry::getInstance().registerService(new ThermostatWebService(*thermo));
  handler->init();
  delay(100);
  thermo->init(thermoConfig);
  delay(100);
}

void loop()
{
  TickNotifier::getInstance().notify();
}