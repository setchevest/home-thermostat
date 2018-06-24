#ifndef ThermostatWebService_h
#define ThermostatWebService_h
#include <IO/WebService.h>
#include <IO/JsonResponse.h>

#define THERMOSTAT_INFO_COMMAND ""
#define THERMOSTAT_ON_COMMAND "on"
#define THERMOSTAT_OFF_COMMAND "off"
#define THERMOSTAT_MANUAL_COMMAND "manual"
#define THERMOSTAT_AUTOMATIC_COMMAND "auto"

class ThermostatWebService : public WebService
{
  private:
    Thermostat &thermostat;

  protected:
    /*virtual*/ void executeCommand(HttpCommand &command, Client &client)
    {
        if (isRoute(command.route, THERMOSTAT_ON_COMMAND))
            thermostat.heaterOn();
        else if (isRoute(command.route, THERMOSTAT_OFF_COMMAND))
            thermostat.heaterOff();
        else if (isRoute(command.route, THERMOSTAT_MANUAL_COMMAND))
            thermostat.setManualMode(true);
        else if (isRoute(command.route, THERMOSTAT_AUTOMATIC_COMMAND))
            thermostat.setManualMode(false);

        JsonResponse response(thermostat);
        response.flush(client);
    }

  public:
    ThermostatWebService(Thermostat &thermostat_) : WebService(),
                                                    thermostat(thermostat_)

    {
    }

    ~ThermostatWebService()
    {
    }
    /*virtual*/ bool canExecute(HttpCommand &command)
    {
        return isRoute(command.route, THERMOSTAT_INFO_COMMAND) ||
               isRoute(command.route, THERMOSTAT_ON_COMMAND) ||
               isRoute(command.route, THERMOSTAT_OFF_COMMAND) ||
               isRoute(command.route, THERMOSTAT_MANUAL_COMMAND) ||
               isRoute(command.route, THERMOSTAT_AUTOMATIC_COMMAND);
    }
};

#endif