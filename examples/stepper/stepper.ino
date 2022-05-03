#include "AxisControl.h"

class AccelStepper : public Axis
{
private:
    int pulse_pin;
    int dir_pin;

public:
    int steps_taken = 0;
    int pulse_width = 0;
    int last_switch = 0;
    byte curr_dir = LOW;
    byte curr_state = LOW;

    AccelStepper(int pul, int dir)
    {
        pulse_pin = pul;
        dir_pin = dir;
        pinMode(pulse_pin, OUTPUT);
        pinMode(dir_pin, OUTPUT);
    }

    unsigned int getMillis()
    {
        return millis();
    }

    void computePosition(double *axis_position)
    {
        *(axis_position) = steps_taken;
    }

    void updateMotorSpeed(double *axis_speed)
    {
        curr_dir = *(axis_speed) > 0 ? HIGH : LOW;
        digitalWrite(dir_pin, curr_dir);
        pulse_width = (1000 * (*(axis_speed))) / 2;
    }

    void pollMotor()
    {
        unsigned int curr_time = millis();
        if ((curr_time - last_switch) >= pulse_width)// 50% duty cycle
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
}

AccelStepper stepper(11, 10);

void setup()
{
    stepper.setLimitMode(1);
    stepper.setTargetSpeed(40);
    stepper.setTargetAcceleration(2.5);
}

void loop()
{
    stepper.move(150);
    stepper.runToPosition();
    delay(2000);
    stepper.move(-150);
    stepper.runToPosition();
    delay(2000);
}