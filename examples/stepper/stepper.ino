#include "AxisControl.h"
#include "accel_stepper.h"

AccelStepper stepper(11, 10);

void setup()
{
    stepper.setLimitMode(1);
    stepper.setTargetSpeed(40); // Steps Per Second
    stepper.setTargetAcceleration(2.5);// Steps Per Second ^ 2
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