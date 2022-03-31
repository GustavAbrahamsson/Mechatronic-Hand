#include <Arduino.h>
#include <Wire.h>

void write_motor_pos(int addr, uint16_t pos){
    Wire.beginTransmission(addr);   // Start sending to motor with addr
    Wire.write(0x01);               // 0x01 == set position
    Wire.write(pos & 0x00FF);       // write lower 8 bits
    pos = pos>>8;                   // Shift down high bits
    Wire.write(pos);                // write 8 high bits
    Wire.endTransmission();         // Send
}  
