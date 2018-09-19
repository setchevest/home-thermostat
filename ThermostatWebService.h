#ifndef ThermostatWebService_h
#define ThermostatWebService_h
#include <IO/WebService.h>
#include <IO/JsonResponse.h>

#define THERMOSTAT_INFO_COMMAND "getStatus"
#define THERMOSTAT_HEATER_COMMAND "setHeater"
#define THERMOSTAT_MODE_COMMAND "setMode"

class ThermostatWebService : public WebService
{
  private:
    Thermostat &thermostat;

    void run(HttpCommand &command)
    {
        if (isRoute(command.route, THERMOSTAT_HEATER_COMMAND))
        {
            if(memcmp(command.params, "t", 1) == 0)
                thermostat.heaterOn();
            else
                thermostat.heaterOff();
        }
        else if (isRoute(command.route, THERMOSTAT_MODE_COMMAND))
        {
            if(memcmp(command.params, "a", 1) == 0)
                thermostat.setManualMode(false);
            else
                thermostat.setManualMode(true);
        }
    }

  protected:
    /*virtual*/ ActionResponse & executeCommand(HttpCommand &command)
    {
        run(command);
        static JsonResponse response(thermostat);
        Serial.println();
        return response;
    }

  public:
    ThermostatWebService(Thermostat &thermostat_) 
        : WebService(), thermostat(thermostat_) 

    {
    }

    ~ThermostatWebService()
    {
    }
    /*virtual*/ bool canExecute(HttpCommand &command)
    {
        return canExecute(command.route);
    }

    bool canExecute(const char *command)
    {
        return isRoute(command, THERMOSTAT_INFO_COMMAND) ||
               isRoute(command, THERMOSTAT_HEATER_COMMAND) ||
               isRoute(command, THERMOSTAT_MODE_COMMAND);
    }
};

#endif