#ifndef Button_h
#define Button_h

#include <AceButton.h>
using namespace ace_button;

class Button : public AceButton
{
  private:
    void configureButton(uint8_t pin)
    {
        // initialize the pushbutton pin as an input:
        pinMode(pin, INPUT);
        init(pin, LOW);
        ButtonConfig *buttonConfig = getButtonConfig();
        buttonConfig->setEventHandler(buttonEventHandler);
        buttonConfig->setFeature(ButtonConfig::kFeatureClick);
        buttonConfig->setFeature(ButtonConfig::kFeatureDoubleClick);
        buttonConfig->setFeature(ButtonConfig::kFeatureLongPress);
    }

    static void buttonEventHandler(AceButton *button, uint8_t eventType, uint8_t buttonState)
    {
        switch (eventType)
        {
        case AceButton::kEventClicked:
            Serial.print(F("Free Memory: "));
            Serial.println(Environment::getFreeMemory());
            break;
        case AceButton::kEventDoubleClicked:
            break;
        case AceButton::kEventLongPressed:
            break;
        }
    }

  public:
    Button(uint8_t pin) 
    {
        configureButton(pin);
    }
    ~Button() {}

    void update()
    {
        check();
    }
};

#endif