#include <Arduino.h>
#include <potlib.h>

POTSwitch::POTSwitch(int POT1, int POT2)
{   // constructor (def __init__())
    this->pot1 = POT1;
    this->pot2 = POT2;
    this->loop = 0;
    this->deadzone = 1024;
    

}

int POTSwitch::decidePot(){

    int deg1 = analogRead(this->pot1);
    int deg2 = analogRead(this->pot2);

    if (deg1==0){
        return deg2 + (this->deadzone/2);
    }

    else {
        return deg1;
        }
    }

  

