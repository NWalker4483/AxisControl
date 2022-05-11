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
}

void loop()
{
    for (int mode = 0; mode < 2; mode++)
    {
        test.setLimitMode(mode);
        unsigned int start = millis();
        int iter = 10000;
        for (int i = 0; i < iter; i++)
        {
            test.run();
        }
        unsigned int stop = millis();
        float per_iter = (stop - start) / iter;
        Serial.print("\nMode ");
        Serial.print(mode);
        Serial.print("\nAverage over ");
        Serial.print(iter);
        Serial.print(" iterations:");
        Serial.print("\n\t HZ: ");
        Serial.print(per_iter);
        Serial.print("\n\t Loop Time: ");
        Serial.print(per_iter);
    }
    delay(2000);
}