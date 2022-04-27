#ifndef POTLIB_H
#define POTLIB_H

class POTSwitch
{

    private:
        int pot1;
        int pot2;
        int loop = 0;
        int previous;
        int old_deg1;
        int old_deg2;
        float filter_constant;

        int filter(int current, int previous);

    public:
        POTSwitch(int POT1, int POT2, float filter_constant);
        int16_t decidePot();
};

#endif