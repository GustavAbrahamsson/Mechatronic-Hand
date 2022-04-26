#include <Arduino.h>
#include <Wire.h>
#include "MotorControl.h"

MotorControl::MotorControl(uint8_t address)
{
    this->addr = address;
}


void MotorControl::pos_raw_write(int16_t pos){
    Wire.beginTransmission(this->addr);   // Start sending to motor with addr
    Wire.write(8);                  // 0x08 == set raw position
    uint8_t low = pos;
    uint8_t hi = (pos>>8);

    Wire.write(low);                // write lower 8 bits
    Wire.write(hi);                 // write 8 high bits
    Wire.endTransmission();         // Send
}

uint8_t MotorControl::current_read(){
    Wire.beginTransmission(this->addr);
    Wire.write(3);                  // 3 == read current
    Wire.endTransmission();

    Wire.requestFrom(this->addr, 1u);    //Read 1 byte
    return(Wire.read());
}

int16_t MotorControl::pos_raw_read(){
    Wire.beginTransmission(this->addr);
    Wire.write(7);                  // 7 == read raw position
    Wire.endTransmission();

    Wire.requestFrom(this->addr, 2u);    //Read 2 bytes
    uint8_t lo = Wire.read();
    uint8_t hi = Wire.read();
    return((hi<<8) | lo);
}

int16_t MotorControl::PID_output_read(){
    Wire.beginTransmission(this->addr);
    Wire.write(10);                  // 10 == read PID output
    Wire.endTransmission();

    Wire.requestFrom(this->addr, 2u);    //Read 2 bytes
    uint8_t lo = Wire.read();
    uint8_t hi = Wire.read();
    return((hi<<8) | lo);
}

