#ifndef WebServiceFactory_h
#define WebServiceFactory_h
#include <IO/ActionResponse.h>
#include <IO/WebService.h>
#include <IO/DefaultWebService.h>

#ifndef MAX_SERVICES
#define MAX_SERVICES 1
#endif

class WebServiceRegistry
{
  private:
    WebService *services[MAX_SERVICES];
    size_t servicesQty = 0;
    WebService *defaultService;

    WebServiceRegistry(/* args */)
    {
        defaultService = new DefaultWebService();
    }

    ~WebServiceRegistry()
    {
        reset();
        delete defaultService;
    }

  public:
    static WebServiceRegistry &getInstance()
    {
        static WebServiceRegistry instance; // Guaranteed to be destroyed.
        return instance;                    // Instantiated on first use.
    }

    WebServiceRegistry(WebServiceRegistry const &) = delete;
    void operator=(WebServiceRegistry const &) = delete;

    WebService *getService(HttpCommand &command)
    {
        for (size_t i = 0; i < servicesQty; i++)
            if (services[i]->canExecute(command))
                return services[i];

        return defaultService;
    }

    void registerService(WebService *service)
    {
        if (servicesQty + 1 <= MAX_SERVICES)
            services[servicesQty++] = service;
#ifdef LOGGING
        else
            Serial.println(F("To many services registered. Please increase array size to do so."));
#endif
    }

    void reset()
    {
        for (size_t i = 0; i < servicesQty; i++)
            delete services[i];
    }
};
#endif