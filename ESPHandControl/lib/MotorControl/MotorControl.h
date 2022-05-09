#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H

class MotorControl
{
private:
    /* data */

    int16_t minPosition;
    int16_t maxPosition;

public:
    int16_t minAngle;
    int16_t maxAngle;
    uint8_t addr;
    MotorControl(uint8_t address, int16_t minPosition, int16_t maxPosition, int16_t minAngle, int16_t maxAngle);
    void pos_raw_write(int16_t pos);
    void angle_write(int16_t pos);
    uint8_t current_read();
    int16_t pos_raw_read();
    int16_t PID_output_read();
};

#endif