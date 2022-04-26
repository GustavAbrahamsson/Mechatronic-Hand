#ifndef POTLIB_H
#define POTLIB_H

class POTSwitch
{

    private:
        int pot1;
        int pot2;
        int loop = 0;
        int previous;
        int previousDeg1;


    public:
        POTSwitch(int POT1, int POT2);
        int16_t decidePot();
};

#endif