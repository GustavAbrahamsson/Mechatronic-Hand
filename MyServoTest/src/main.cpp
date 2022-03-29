#include <Arduino.h>
#include <stdio.h>
#define MOTOR1 PIN_PA4
#define MOTOR2 PIN_PA5
#define LED PIN_PA2 
#define POT PIN_PA7
#define SHUNT PIN_PA6

int rpm;

void setup() {
  // put your setup code here, to run once:

  pinMode(MOTOR1, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(POT, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:

  digitalWrite(LED, HIGH);
  delay(3000);
  digitalWrite(LED, LOW);
  delay(1000);
  digitalWrite(MOTOR1, HIGH);
  delay(1000);
  analogWrite(MOTOR1, 50);
  delay(1000);
  analogWrite(MOTOR1, 255);
  delay(1000);
  digitalWrite(MOTOR1, LOW);

  rpm = analogRead(POT);



}