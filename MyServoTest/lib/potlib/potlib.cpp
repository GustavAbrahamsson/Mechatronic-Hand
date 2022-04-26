#include <Arduino.h>
#include <potlib.h>

POTSwitch::POTSwitch(int POT1, int POT2)
{   // constructor (def __init__())
    this->pot1 = POT1;
    this->pot2 = POT2;
    this->loop = 0;
}

int16_t POTSwitch::decidePot(){

    // read both potentiometers
    int deg1 = analogRead(this->pot1);
    int deg2 = analogRead(this->pot2);

    // Check if we have looped around from 0 to 1023 
    if((this->previousDeg1 < 232) && (deg1 > 790)){
        this->loop--;
    } 
    // Check if we have looped around from 1023 to 0
    else if((this->previousDeg1 > 790) && (deg1 < 232)){
        this->loop++;
    }

    int value;

    // Handle deadzone
    if (deg1==1023){
        value = deg2 + 512;
    }else{
        value = deg1;
    }

    // Update previous value
    this->previousDeg1 = deg1;

    // calculate value with loops
    value = this->loop*1024+value;

    // filter out noise
    //value = this->previous*0.9 + (value)*0.1;

    return value;    

}

  

