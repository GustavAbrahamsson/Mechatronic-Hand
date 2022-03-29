class MyPID
{

private:
    float integral = 0;
    float previous_error = 0;
    float Kp;
    float Ki;
    float Kd;
    

public:
    MyPID(float Kp, float Ki, float Kd);
    float NextStep(float dt, float setpoint, float current_value);
};
