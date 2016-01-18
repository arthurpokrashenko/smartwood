#include <CapacitiveSensor.h>

CapacitiveSensor capSensor = CapacitiveSensor(4,2); // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired
int ledPin = 10;
int ledState = LOW; // the current state of the output pin
int buttonState; // the current reading from the input pin
int lastButtonState = HIGH; // the previous reading from the input pin
int capacitiveCheckPoint = 150; // sensivity
int brightness = 0;
long lastDebounceTime = 0; // the last time the output pin was toggled
long debounceDelay = 50; // the debounce time; increase if the output flickers

void setup() {
  pinMode(ledPin, OUTPUT);
  capSensor.set_CS_AutocaL_Millis(0xFFFFFFFF); // turn off autocalibrate on channel 1 - just as an example
  Serial.begin(9600);
}

void loop() {
  long total = capSensor.capacitiveSensor(30);
  int reading = toggle(total);

  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH) {
        ledState = !ledState;
      }
    }
  }

  if (ledState == HIGH) {
    brightness = min(254, ++brightness);
    analogWrite(ledPin, brightness);
  } else {
    brightness = max(0, --brightness);
    analogWrite(ledPin, brightness);
  }
  Serial.print("#brightness");
  Serial.print(brightness); // print sensor output 1
  Serial.print("\n");
//  digitalWrite(ledPin, ledState);

  // save the reading. Next time through the loop,
  // it'll be the lastButtonState:
  lastButtonState = reading;
  //updateLed(ledPin,total);

  Serial.print(total); // print sensor output 1
  Serial.print("\n"); // tab character for debug windown spacing

  delay(10); // arbitrary delay to limit data to serial port
}

boolean toggle(long value) {
  if (value > capacitiveCheckPoint) {
    return HIGH;
  } else {
    return LOW;
  }
}