#include "AxisControl.h"
// A Fully Non-Blocking AccelStepper Implementation

class AxisStepper : public Axis
{
private:
    int pulse_pin;
    int dir_pin;
    int steps_taken = 0;
    int pulse_width = 0;
    int last_switch = 0;

public:
    byte curr_dir = LOW;
    byte curr_state = LOW;

    AccelStepper(int pul, int dir)
    {
        pulse_pin = pul;
        dir_pin = dir;
        pinMode(pulse_pin, OUTPUT);
        pinMode(dir_pin, OUTPUT);
        setResolution(0);
    }

    unsigned int getMillis()
    {
        return millis();
    }

    void computePosition()
    {
        setPosition(steps_taken);
    }

    void updateMotorSpeed(double axis_speed)
    {
        curr_dir = axis_speed > 0 ? HIGH : LOW;
        digitalWrite(dir_pin, curr_dir);
        pulse_width = (1000 * (axis_speed)) / 2;
    }

    void pollMotor()
    {
        unsigned int curr_time = millis();
        if ((curr_time - last_switch) >= pulse_width) // 50% duty cycle
        {
            if (curr_state == HIGH)
            {
                steps_taken += (curr_dir == HIGH) ? 1: -1;  
            }
            curr_state = !curr_state;
            last_switch = curr_time;
            digitalWrite(pulse_pin, curr_state);
        }
    }
};