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
uint16_t setpoint = 150;
float current_value;

uint8_t command

MyPID MyPIDD(Kp, Ki, Kd);


void requestEvent(){
  Wire.write(3/4);
}

void recieveEvent(int numbytes){
  command = Wire.read();

  uint8_t buff[numBytes-1];
  int i = 0
  while(Wire.available()){
    buff[i] = Wire.read()
  }

  switch (command)
  {
  case 0x01:
    // Set motor position
    setpoint = buff[0];
    setpoint += buff[0]<<8;
    break;
  
  default:
    break;
  }

}


void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  pinMode(POT, INPUT);
  pinMode(MOTOR1, OUTPUT);
  pinMode(MOTOR2, OUTPUT);
  Wire.begin(I2CAddress);
  Wire.onRequest(requestEvent);
  Wire.onRecieve(recieveEvent)
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