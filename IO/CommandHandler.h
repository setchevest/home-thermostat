#ifndef CallbackCommandHandler_h
#define CallbackCommandHandler_h
#include <IO/CallbackCommand.h>
#include <Common/LinkedList.h>

class CallbackCommandHandler
{
private:
    LinkedList<CallbackCommand*> commands;
public:
    CallbackCommandHandler(/* args */)
        :commands(LinkedList<CallbackCommand*>()) 
    {}

    ~CallbackCommandHandler() { }

    void add(CallbackCommand &command)
    {   
        commands.add(&command);
    }
};

#endif