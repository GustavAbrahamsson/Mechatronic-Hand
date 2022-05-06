#include <Arduino.h>
#include <potlib.h>

POTSwitch::POTSwitch(int POT1, int POT2, float filter_constant)
{   // constructor (def __init__())
    this->pot1 = POT1;
    this->pot2 = POT2;
    this->filter_constant = filter_constant;
}

int16_t POTSwitch::decidePot(){

    // read both potentiometers
    int deg1 = analogRead(this->pot1);
    int deg2 = analogRead(this->pot2);

    // filter inputs, do not filter changes over 512 (wrap around)
    if (deg1 - this->old_deg1 > 512) {
        // wrapped around from 0 to 1023
        deg1 = this->filter(deg1, this->old_deg1+1024); 
    } else if (this->old_deg1 - deg1 > 512) {
        // wrapped around from 1023 to 0
        deg1 = this->filter(deg1, this->old_deg1-1024);  
    }else{
        deg1 = this->filter(deg1, this->old_deg1);
    }

    if (deg2 - this->old_deg2 > 512) {
        // wrapped around from 0 to 1023
        deg2 = this->filter(deg2, this->old_deg2+1024);
    } else if (this->old_deg2 - deg2 > 512) {
        // wrapped around from 1023 to 0
        deg2 = this->filter(deg2, this->old_deg2-1024);
    }else {
        deg2 = this->filter(deg2, this->old_deg2);
    }

    // update old values
    this->old_deg1 = deg1;
    this->old_deg2 = deg2;

    // Handle deadzone by averaging both potentiometers
    if(deg1 < deg2){
        deg2 -= 512;
    }else{
        deg2 += 512;
    }

    int value = (deg1 + deg2)/2;
 
    // Check if we have looped around from 0 to 1023 
    if((this->previous < 255) && (value > 765)){
        this->loop--;
    } 
    // Check if we have looped around from 1023 to 0
    else if((this->previous > 765) && (value < 255)){
        this->loop++;
    }

    // Update previous value
    this->previous = value;


    // calculate value with loops
    value = this->loop*1024+value;

    return value;

}

// method for filtering noise
// Using exponentially weighted moving average filter (IIR)
int POTSwitch::filter(int current, int previous){
    float alpha = this->filter_constant;
    return (int) (alpha*current + (1-alpha)*previous);
}

void POTSwitch::reset(){
    this->old_deg1 = analogRead(this->pot1);
    this->old_deg2 = analogRead(this->pot2);

    this->previous = this->old_deg1;
}
