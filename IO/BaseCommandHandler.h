#include <Common/Interfaces/IObserver.h>
#include <Configuration.h>

class BaseCommandHandler : public IObserver
{
protected:
    Configuration::ServerConfig &serverConfig;
    
    void onTick() 
    {

    }

public:
    CommandHandler(Configuration::ServerConfig &serverConfig_)
    serverConfig(serverConfig_) { }

    ~CommandHandler() { }

    void virtual init()
    {
        TickNotifier::getInstance().attach(this);
    }
    
    void update(const char *command)
    {
        if (strcmp(command, TickNotifier::getInstance().tickCommand) == 0)
        {
            onTick();
        }
    }
};