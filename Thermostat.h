#ifndef Thermostat_h
#define Thermostat_h

#include <Configuration.h>
#include <HeaterController.h>
#include <ZoneList.h>
#include <Common/Timer.h>
#include <Common/Callback.h>
#include <Common/Serializable.h>
#include <Common/Environment.h>
#include <Common/Interfaces/IObserver.h>
#include <Ethernet.h>
#include <IO/JsonResponse.h>
// #include <HttpClient.h>

using namespace Configuration;

class Thermostat : public Serializable
{
private:
  HeaterController *heater = nullptr;
  IList<TemperatureZone *> *zoneList = nullptr;
  Timer *timer = nullptr;
  ThermostatConfig config;
  RequestConfig &server;
  bool manualModeEnabled = true;
  int controlZoneId = -1;
  // Number of milliseconds to wait without receiving any data before we give up
  const unsigned int kNetworkTimeout = 30 * 1000;
  // Number of milliseconds to wait if no data is available before trying again
  const unsigned int kNetworkDelay = 200;
  void getDataFromSensors();
  void check();
  void cleanup();
  void invalidateHeaterStatus();

public:
  Thermostat(RequestConfig &server_);
  ~Thermostat();
  void init(ThermostatConfig config);
  void toJson(JsonObject &root);
  void toggleHeater();
  void heaterOn();
  void heaterOff();
  void setManualMode(bool manualModeEnabled_);
};

#endif