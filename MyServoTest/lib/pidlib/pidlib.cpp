
class MyPID
{

    private:
        float integral;
        float previous_error;
        float Kp;
        float Ki;
        float Kd;
        

    public:
        MyPID(float Kp, float Ki, float Kd);
        float NextStep(float dt, float setpoint, float current_value);
};

MyPID::MyPID(float Kp, float Ki, float Kd)
{   // constructor (def __init__())
    this->Kp = Kp;
    this->Ki = Ki;
    this->Kd = Kd;
    this->integral = 0;
    this->previous_error=0;


}
float MyPID::NextStep(float dt, float setpoint, float current_value){

    float current_error = setpoint - current_value;
    this->integral += (current_error*(dt/1000));
    float derivative = (current_error - this->previous_error)/(dt/1000);
    float output = this->Kp*current_error + this->Ki*this->integral + this->Kd*derivative;
    this->previous_error = current_error;
    return output;
}
