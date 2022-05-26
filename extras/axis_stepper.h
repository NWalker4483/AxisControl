#include "AxisControl.h"
// A Fully Non-Blocking AccelStepper Implementation

class AxisStepper : public Axis
{
private:
    int pulse_pin;
    int dir_pin;
    int pulse_width = 0;
    int last_switch = 0;
    bool stopped = 0; 
public:
    int steps_taken = 0;//TODO: Make private with setter for ZT
    byte curr_dir = LOW;
    byte curr_state = LOW;

    AxisStepper(int pul, int dir)
    {
        pulse_pin = pul;
        dir_pin = dir;
        pinMode(pulse_pin, OUTPUT);
        pinMode(dir_pin, OUTPUT);
        setResolution(2);
    }

    unsigned int getMillis()
    {
        return millis();
    }

    void computePosition()
    {
        setPosition(steps_taken);
    }

    void clearSteps(){
        steps_taken  = 0;
    }

    void updateMotorSpeed(double axis_speed)
    {
        curr_dir = axis_speed > 0 ? HIGH : LOW;
        digitalWrite(dir_pin, curr_dir);
        stopped = (axis_speed == 0);
        pulse_width = (1000L / fabs(axis_speed)) / 2;
    }

    void pollMotor()
    {
        unsigned int curr_time = micros();
        if (stopped) return;
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