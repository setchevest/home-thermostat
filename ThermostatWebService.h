#ifndef ThermostatWebService_h
#define ThermostatWebService_h
#include <IO/WebService.h>
#include <IO/JsonResponse.h>

class ThermostatWebService : public WebService
{
  private:
    Thermostat &thermostat;

  protected:
    /*virtual*/ void executeCommand(HttpCommand &command, Client &client)
    {
        if (isRoute(command.route,"on"))
            thermostat.heaterOn();
        else if (isRoute(command.route,"off"))
            thermostat.heaterOff();
        else
            return;
        
        JsonResponse response(thermostat);
        response.flush(client);
    }

  public:
    ThermostatWebService(Thermostat &thermostat_) : WebService(),
                                                    thermostat(thermostat_) {}

    /*virtual*/ bool canExecute(HttpCommand &command)
    {
        return isRoute(command.route,"on") || isRoute(command.route,"off");
    }
};

#endif