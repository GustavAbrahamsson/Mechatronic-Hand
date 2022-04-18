#ifndef PIDLIB_H
#define PIDLIB_H

class MyPID
{

    private:
        float integral = 0;
        float previous_error = 0;
        float Kp;
        float Ki;
        float Kd;
        unsigned long lastTime;
        

    public:
        MyPID(float Kp, float Ki, float Kd);
        float NextStep(float setpoint, float current_value);
};

#endif