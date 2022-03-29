#include <Arduino.h>
#include <Wire.h>
#include "pidlib.h"


#define MOTOR1 PIN_PA4
#define MOTOR2 PIN_PA5
#define LED PIN_PA2 
#define POT PIN_PA7
#define I2CAddress 8
#define DT 20

float Kp=0.1, Ki=0, Kd=0;
float setpoint = 150;
float current_value;


MyPID MyPIDD(Kp, Ki, Kd);


void requestEvent(){
  Wire.write(3/4);
}



void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  pinMode(POT, INPUT);
  pinMode(MOTOR1, OUTPUT);
  pinMode(MOTOR2, OUTPUT);
  Wire.begin(I2CAddress);
  Wire.onRequest(requestEvent);
};

void loop() {
  // put your main code here, to run repeatedly:

  // Measure deg from potentiometer
  float deg = (float) analogRead(POT);

  // Use PID tp determine output
  uint8_t output = MyPIDD.NextStep(DT, setpoint, deg);


  if (output < 0){
    analogWrite(MOTOR2, 0);
    analogWrite(MOTOR1, output);
  }
  else{
    analogWrite(MOTOR1, 0);
    analogWrite(MOTOR2, output);
  }

  delay(DT);

}