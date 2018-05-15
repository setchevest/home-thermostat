
class ControlStrategy
{
private:
    /* data */
public:
    ControlStrategy();
    ~ControlStrategy();
protected:
    virtual bool isOn() = 0;
};

ControlStrategy::ControlStrategy()
{
}

ControlStrategy::~ControlStrategy()
{
}
