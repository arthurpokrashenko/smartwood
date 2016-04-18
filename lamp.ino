#include <CapacitiveSensor.h>

CapacitiveSensor capSensor = CapacitiveSensor(4,2);
int ledPin = 10;
int ledState = LOW;
int buttonState;
int lastButtonState = HIGH;
int capacitiveCheckPoint = 150;
int brightness = 0;
long lastDebounceTime = 0;
long debounceDelay = 50;

void setup() {
  pinMode(ledPin, OUTPUT);
  capSensor.set_CS_AutocaL_Millis(0xFFFFFFFF);
  Serial.begin(9600);
}

void loop() {
  long total = capSensor.capacitiveSensor(30);
  int reading = toggle(total);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

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
  lastButtonState = reading;
  delay(10);
}

boolean toggle(long value) {
  if (value > capacitiveCheckPoint) {
    return HIGH;
  } else {
    return LOW;
  }
}
