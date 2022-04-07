#include <Arduino.h>
#include <Wire.h>
#include "pidlib.h"

// Comment the following line for ATtiny
#define NANO

// Nano Pins
#ifdef NANO
  #define MOTOR1 5
  #define MOTOR2 6
  #define LED 13 
  #define POT A0
  #define SPEEDPIN 7
#endif

// ATtiny pins
#ifndef NANO
  #define MOTOR1 PIN_PA4
  #define MOTOR2 PIN_PA5
  #define LED PIN_PA2 
  #define POT PIN_PA7
#endif

#define I2CAddress 8
#define DT 20


float Kp=7, Ki=0, Kd=0.05;
uint16_t setpoint = 200;
float current_value;

uint8_t command;
int output;
int deg;

MyPID MyPIDD(Kp, Ki, Kd);


void requestEvent(){
  Wire.write(deg);
}

// void recieveEvent(int numbytes){
//   // command = Wire.read();

//   // uint8_t buff[numbytes-1];
//   // int i = 0;
//   // while(Wire.available()){
//   //   buff[i] = Wire.read();
//   // }

//   // switch (command)
//   // {
//   // case 0x01:
//   //   // Set motor position
//   //   setpoint = buff[0];
//   //   setpoint += buff[0]<<8;
//   //   break;
  
//   // default:
//   //   break;
//   // }

// }


void setup() {
  // put your setup code here, to run once:
  #ifdef NANO
    Serial.begin(115200);
  #endif
  pinMode(POT, INPUT);
  pinMode(MOTOR1, OUTPUT);
  pinMode(MOTOR2, OUTPUT);
  Wire.begin(I2CAddress);
  Wire.onRequest(requestEvent);
  // Wire.onReceive(recieveEvent);
};

void loop() {

  // Measure deg from potentiometer
  deg = analogRead(POT)/4;


  // Use PID tp determine output
  output = MyPIDD.NextStep(DT, setpoint, deg);

  #ifdef NANO
    Serial.print("DEG:\t");
    Serial.print(deg);
    Serial.print("\tOUTPUT:\t");
    Serial.println(output);
  #endif

  if (output < 0){
    analogWrite(MOTOR2, 0);

    #ifndef NANO
      analogWrite(MOTOR1, -output);
    #endif

    #ifdef NANO
      analogWrite(MOTOR1, 255);
      analogWrite(SPEEDPIN, -output);
    #endif
  }
  else{
    analogWrite(MOTOR1, 0);

    #ifndef NANO
      analogWrite(MOTOR2, -output);
    #endif

    #ifdef NANO
      analogWrite(MOTOR2, 255);
      analogWrite(SPEEDPIN, -output);
    #endif
  }

  delay(DT);

}