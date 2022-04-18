#include "Arduino.h"
#include "pidlib.h"

MyPID::MyPID(float Kp, float Ki, float Kd)
{   // constructor (def __init__())
    this->Kp = Kp;
    this->Ki = Ki;
    this->Kd = Kd;
    this->integral = 0;
    this->previous_error=0;


}

float MyPID::NextStep(float setpoint, float current_value){

    // Calculate delta time (seconds) 
    float dt = (micros() - this->lastTime)/ 1000000.0;
    this->lastTime = micros();

    // Calculate error
    float current_error = setpoint - current_value;
    // Calculate integral
    this->integral += (current_error*(dt));
    // Calculate derivative
    float derivative = (current_error - this->previous_error)/(dt);
    // Calculate output
    float output = this->Kp*current_error + this->Ki*this->integral + this->Kd*derivative;
    this->previous_error = current_error;
    
    // Constrain output
    if (output > 255){
        output = 255;
    }
    else if (output < -255){
        output = -255;
    }
    return output;
}
