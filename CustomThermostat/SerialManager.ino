#include <Arduino.h>

String inputString = "";        // a String to hold incoming data
bool stringComplete = false; // whether the string is complete

void checkSerialData()
{
  if (stringComplete)
  {
    Serial.println("Thanks for providing feedback");
    Serial.println(inputString);
    stringComplete = false;
  }
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    if (inChar == '#') {
      stringComplete = true;
    }
    else
      inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:

  }
}