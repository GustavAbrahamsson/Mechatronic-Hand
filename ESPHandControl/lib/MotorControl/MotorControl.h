#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H

class MotorControl
{
private:
    /* data */
public:
    uint8_t addr;
    MotorControl(uint8_t address);
    void pos_raw_write(int16_t pos);
    uint8_t current_read();
    int16_t pos_raw_read();
    int16_t PID_output_read();
};

#endif