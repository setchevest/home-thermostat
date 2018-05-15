#include <AceButton.h>
#include <ContextableThread.h>
#include <Callback.h>
#include <Thermostat.h>
#include <Environment.h>
#include <Serializable.h>

using namespace ace_button;

#define ENABLE_SERIAL 1

#define ONOFFBUTTON_PIN 3 // the number of the pushbutton pin
#define STATUSLED_PIN 4   // the number of the LED pin
#define RELAY_PIN 2

#define ZONE1SENSOR_PIN 8
#define ZONE2SENSOR_PIN 9

const int k_mintimebetweenreeds = 5000;

AceButton button;
Thermostat thermo = Thermostat(RELAY_PIN, STATUSLED_PIN);
ContextableThread thread1 = ContextableThread(new Callback(thermo,&Thermostat::updateStatus), k_mintimebetweenreeds);

void setup()
{
  Serial.begin(9600);
  thermo.addZone(1,"Zone 1",ZONE1SENSOR_PIN);
  thermo.addZone(2,"Zone 2",ZONE2SENSOR_PIN);
  configureButton();
}

void loop()
{
  thread1.check();
  button.check();
}

void configureButton()
{
  // initialize the pushbutton pin as an input:
  pinMode(ONOFFBUTTON_PIN, INPUT);
  button.init(ONOFFBUTTON_PIN, LOW);
  ButtonConfig *buttonConfig = button.getButtonConfig();
  buttonConfig->setEventHandler(buttonEventHandler);
  buttonConfig->setFeature(ButtonConfig::kFeatureClick);
  buttonConfig->setFeature(ButtonConfig::kFeatureDoubleClick);
  buttonConfig->setFeature(ButtonConfig::kFeatureLongPress);
  buttonConfig->clearFeature(ButtonConfig::kFeatureClick);
  buttonConfig->clearFeature(ButtonConfig::kFeatureSuppressAfterClick);
  buttonConfig->setFeature(ButtonConfig::kFeatureDoubleClick);
  buttonConfig->setFeature(ButtonConfig::kFeatureLongPress);
}

void buttonEventHandler(AceButton *button, uint8_t eventType, uint8_t buttonState)
{

  switch (eventType) {
    case AceButton::kEventClicked:
      Serial.print(F("Free Memory: "));
      Serial.println(Environment::getFreeMemory());
      break;
    case AceButton::kEventDoubleClicked:
      thermo.toggleHeater();
      break;
    case AceButton::kEventLongPressed:
      printTempDataToSerial();
      break;
  }
}


void printTempDataToSerial()
{
  DynamicJsonBuffer jsonBuffer(500);
  JsonObject &root = jsonBuffer.createObject();
  thermo.toJson(root);
  Serial.println();
  root.prettyPrintTo(Serial);
  Serial.println();
}

