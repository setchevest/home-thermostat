#include <AceButton.h>
#include <Thermostat.h>

// #include <SPI.h>
// #include <aREST.h>
// #include <avr/wdt.h>

using namespace ace_button;

#define ONOFFBUTTON_PIN 3 // the number of the pushbutton pin
#define STATUSLED_PIN 4   // the number of the LED pin
#define RELAY_PIN 2
#define ZONE1SENSOR_PIN 8
#define ZONE2SENSOR_PIN 9

const int k_mintimebetweenreeds = 5000;

// Create aREST instance
// aREST rest = aREST();

AceButton button;
Thermostat thermo = Thermostat(RELAY_PIN, STATUSLED_PIN);
ContextableThread thread1 = ContextableThread(*new Callback(thermo, &Thermostat::keepAlive), k_mintimebetweenreeds);

void setup()
{
  Serial.begin(115200);
  
  thermo.addZone(1, "Zone 1", ZONE1SENSOR_PIN);
  thermo.addZone(2, "Zone 2", ZONE2SENSOR_PIN);
  // configureRESTApi();
  configureButton();
}

String inputString = "";        // a String to hold incoming data
boolean stringComplete = false; // whether the string is complete

void loop()
{
  thread1.check();
  button.check();
  if (stringComplete)
  {
    Serial.println("Thanks for providing feedback");
    Serial.println(inputString);
    stringComplete = false;
  }
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
}

void buttonEventHandler(AceButton *button, uint8_t eventType, uint8_t buttonState)
{
  switch (eventType)
  {
  case AceButton::kEventClicked:
    Serial.print(F("Free Memory: "));
    Serial.println(Environment::getFreeMemory());
    break;
  case AceButton::kEventDoubleClicked:
    thermo.toggleHeater();
    break;
  case AceButton::kEventLongPressed:
    printTempData(Serial);
    break;
  }
}

void printTempData(Print &printer)
{
  DynamicJsonBuffer jsonBuffer(500);
  JsonObject &root = jsonBuffer.createObject();
  thermo.toJson(root);
  Serial.println();
  root.prettyPrintTo(printer);
  Serial.println();
}

// void serialEvent() {
//   while (Serial.available()) {
//     // get the new byte:
//     char inChar = (char)Serial.read();
//     // add it to the inputString:
//     if (inChar == '#') {
//       stringComplete = true;
//     }
//     else
//       inputString += inChar;
//     // if the incoming character is a newline, set a flag so the main loop can
//     // do something about it:

//   }
// }
