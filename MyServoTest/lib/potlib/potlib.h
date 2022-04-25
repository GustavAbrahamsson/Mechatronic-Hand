#ifndef POTLIB_H
#define POTLIB_H

class POTSwitch
{

    private:
        int pot1;
        int pot2;
        int loop = 0;
        int deadzone = 1024;
        

    public:
        POTSwitch::POTSwitch(int POT1, int POT2);
        int POTSwitch::decidePot();
};

#endif