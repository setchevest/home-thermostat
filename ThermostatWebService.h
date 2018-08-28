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

    void run(const char *topic)
    {
        if (isRoute(topic, THERMOSTAT_ON_COMMAND))
            thermostat.heaterOn();
        else if (isRoute(topic, THERMOSTAT_OFF_COMMAND))
            thermostat.heaterOff();
        else if (isRoute(topic, THERMOSTAT_MANUAL_COMMAND))
            thermostat.setManualMode(true);
        else if (isRoute(topic, THERMOSTAT_AUTOMATIC_COMMAND))
            thermostat.setManualMode(false);
    }

  protected:
    /*virtual*/ void executeCommand(HttpCommand &command, Client &client)
    {
        run(command.route);
        JsonResponse response(thermostat);
        response.flush(client);
    }

    /*virtual*/ void executeMessage(char *topic, byte *payload, unsigned int length)
    {
        run(topic);
    }

  public:
    ThermostatWebService(Thermostat &thermostat_) 
        : WebService(),thermostat(thermostat_)

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
               isRoute(command, THERMOSTAT_ON_COMMAND) ||
               isRoute(command, THERMOSTAT_OFF_COMMAND) ||
               isRoute(command, THERMOSTAT_MANUAL_COMMAND) ||
               isRoute(command, THERMOSTAT_AUTOMATIC_COMMAND);
    }
};

#endif