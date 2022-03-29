#include <Arduino.h>


void setup() {
  // put your setup code here, to run once:

  pinMode(PIN_PA2, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:

  digitalWrite(PIN_PA2, HIGH);
  delay(3000);
  digitalWrite(PIN_PA2, LOW);
  delay(1000);

}