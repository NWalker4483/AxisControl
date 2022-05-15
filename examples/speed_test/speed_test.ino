#include "AxisControl.h"

class TestAxis : public Axis
{
    unsigned int time = 0;
    unsigned int inc = 10;
    unsigned int getMillis()
    {
        time += inc;
        return time;
    }

    void computePosition()
    {
        setPosition(currentPosition() + (getCMDSpeed() * ((double)inc / 1000L)));
    }

    void updateMotorSpeed(double axis_speeds){};

    void pollMotor(){};
};

TestAxis test;

void setup()
{
    Serial.begin(9600);
    test.setTargetSpeed(1);
    test.setTargetAcceleration(.25);
    test.setTargetJerk(.1);
}

int iter = 1000;
void loop()
{
    for (int mode = 0; mode < 3; mode++)
    {
        test.setLimitMode(mode);
        unsigned int start = millis();
        test.setPosition(0);
        // test.reset();
        test.move(10000);
        for (int i = 0; i < iter; i++)
        {
            test.run();
        }
        unsigned int stop = millis();
        float per_iter = (float)(stop - start) / (float)iter;
        Serial.print("\nMode ");
        Serial.print(mode);
        Serial.print(":\nAverage over ");
        Serial.print(iter);
        Serial.print(" iterations:");
        Serial.print("\n\t HZ: ");
        Serial.print(1000L/per_iter);
        Serial.print("\n\t Time Per Loop (ms): ");
        Serial.print(per_iter);
    }
    delay(2000);
}