#include "AxisControl.h"
#include "extras/accel_stepper.h"

AccelStepper stepper1(1, 2);
AccelStepper stepper2(3, 4);
AccelStepper stepper3(5, 6);
AccelStepper stepper4(7, 8);
AccelStepper stepper5(9, 10);
AccelStepper stepper6(11, 12);

class StepperBot : public MultiAxis<6>
{
    AccelStepper *steppers[6];

    StepperBot()
    {
        steppers[0] = &stepper1;
        steppers[1] = &stepper2;
        steppers[2] = &stepper3;
        steppers[3] = &stepper4;
        steppers[4] = &stepper5;
        steppers[5] = &stepper6;
        for (int i = 0; i < 6; i++) steppers[i]->setLimitMode(0);
    }

    unsigned int getMillis() override { return millis(); };

    void computeAxisPositions(double *axis_positions) override
    {
        // do the linear algebra stuff
        // for (int i = 0; i < 6; i++)
        //     *(axis_positions) = steppers[i]->currentPosition();
    };

    void updateMotorSpeeds(double *axis_speeds) override
    {
        // do the linear algebra stuff
        for (int i = 0; i < 6; i++)
            steppers[i]->setTargetSpeed(*(axis_speeds + i));
    }

    void pollMotors() override
    {
        for (int i = 0; i < 6; i++)
            steppers[i]->pollMotor();
    };
};

StepperBot robot;

void setup()
{

}

void loop()
{

}