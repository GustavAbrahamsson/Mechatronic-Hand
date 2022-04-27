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

float MyPID::nextStep(float setpoint, float current_value){

    // Calculate delta time (seconds) 
    float dt = (micros() - this->lastTime)/ 1000000.0;
    this->lastTime = micros();

    // Calculate error
    float current_error = setpoint - current_value;
    // Calculate integral
    this->integral += (current_error*(dt));
    // integral zero-crossing
    if(
            (this->previous_error >= 0 && current_error <= 0) ||
            (this->previous_error <= 0 && current_error >= 0)
        ){
        this->integral = 0;
    }
    
    // cap integral
    if(this->integral*this->Ki > 255){
        this->integral = 255/this->Ki;
    }
    if(this->integral*this->Ki < -255){
        this->integral = -255/this->Ki;
    }


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
