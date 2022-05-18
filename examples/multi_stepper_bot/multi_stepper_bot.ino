#include "AxisControl.h"
#include "extras/axis_stepper.h"

AxisStepper stepper1(1, 2);
AxisStepper stepper2(3, 4);
AxisStepper stepper3(5, 6);
AxisStepper stepper4(7, 8);
AxisStepper stepper5(9, 10);
AxisStepper stepper6(11, 12);

class StepperBot : public MultiAxis<6>
{
    AxisStepper *steppers[6];

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