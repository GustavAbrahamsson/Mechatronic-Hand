#include <Arduino.h>
#include <Wire.h>
#include "pidlib.h"

// Comment the following line for ATtiny
//#define NANO

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
#define DT 10

float Kp=5, Ki=0, Kd=0.2;
uint16_t setpoint = 500;
uint16_t RawPosition;
int16_t output;

uint16_t minPosition = 0;
uint16_t maxPosition = 0xFFFF;

uint8_t current;
uint8_t maxCurrent = 255;

uint8_t command;
uint16_t deg;

MyPID MyPIDD(Kp, Ki, Kd);

// I2C communication spec.

// Desc.        Command ID      length (bytes)      Read/Write
// I2C addr     0               1                   R
// Position*    1               1                   R
// Setpoint*    2               1                   R/W      
// Current      3               1                   R
// Max Current* 4               1                   R/W
// MinLimit*    5               2                   R/W
// MaxLimit*    6               2                   R/W
// RawPosition  7               2                   R
// RawSetpoint  8               2                   R/W  
// AutoLimits*  9               0                   W

// *Not yet implemented

// Request to read data
void requestEvent(){
  switch (command)
  {
  case 0: //I2C address
    Wire.write(I2CAddress);
    break;

  case 1: //Position
    Wire.write(map(deg, minPosition, maxPosition, 0, 255));
    break;

  case 2: //Setpoint
    Wire.write(map(setpoint, minPosition, maxPosition, 0, 255));
    break;

  case 3: // Current
    Wire.write(current);
    break;

  case 4: // Max Current
    Wire.write(maxCurrent);
    break;

  case 5: // Min limit
    Wire.write((uint8_t) minPosition & 0xFF);
    Wire.write((uint8_t) (minPosition<<8) & 0xFF);
    break;

  case 6: // Max limit
    Wire.write((uint8_t) maxPosition & 0xFF);
    Wire.write((uint8_t) (maxPosition<<8) & 0xFF);
    break;

  case 7: // Raw position
    Wire.write((uint8_t) deg & 0xFF);
    Wire.write((uint8_t) (deg<<8) & 0xFF);
    break;

  case 8: // Raw setpoint
    Wire.write((uint8_t) setpoint & 0xFF);
    Wire.write((uint8_t) (setpoint<<8) & 0xFF);
    break;  


  default:
    break;
  }
}

// Write from controller
void recieveEvent(int numbytes){
  command = Wire.read();

  uint8_t buff[3];
  int i = 0;
  while(Wire.available()){
    buff[i] = Wire.read();
    i++;
  }

  switch (command)
  {
  case 2: // new setpoint
    setpoint = buff[0];
    setpoint += buff[1]<<8;
    break;

  case 4: // Max current
    current = buff[0];
    break;

  case 5: // Min Limit
    minPosition = buff[0];
    minPosition += buff[1]<<8;
    break;

  case 6: // Min Limit
    maxPosition = buff[0];
    maxPosition += buff[1]<<8;
    break;

  case 8: // raw setpoint
    setpoint = buff[0];
    setpoint |= buff[1]<<8;
    if(buff[0] > 100){
      digitalWrite(LED, HIGH);
    }
    break;
 
  default:
    break;
  }

}


void setup() {
  // put your setup code here, to run once:
  #ifdef NANO
    Serial.begin(115200);
  #endif

  // Use higher PWM frequency to remove noise
  #ifndef NANO
    TCA0.SPLIT.CTRLA = TCA_SINGLE_CLKSEL_DIV2_gc | TCA_SINGLE_ENABLE_bm;
  #endif

  pinMode(POT, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(MOTOR1, OUTPUT);
  pinMode(MOTOR2, OUTPUT);
  Wire.begin(I2CAddress);
  Wire.onRequest(requestEvent);
  Wire.onReceive(recieveEvent);
};

void loop() {

  // Measure deg from potentiometer
  deg = analogRead(POT);


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
      analogWrite(MOTOR2, output);
    #endif

    #ifdef NANO
      analogWrite(MOTOR2, 255);
      analogWrite(SPEEDPIN, output);
    #endif
  }

  delay(DT);

}