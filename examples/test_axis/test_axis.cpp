#include "multi_axis.h"
#include "iostream"
#include <unistd.h>
using namespace std;
class TestAxis : public Axis
{
private:
    int pulse_pin;
    int dir_pin;
    int last_switch = 0;

public:
    int curr_dir = 0;
    int curr_state = 0;

    int t;
    unsigned int step_size = 1;
    unsigned int inc = 1;
    unsigned int last_step = 0;
    int steps_taken = 0;
    int step_delay = 0;
    bool stopped = true;

    unsigned int time = 0;

    TestAxis()
    {
    }

    ~TestAxis()
    {
    }

protected:
    unsigned int getMillis()
    {
        time += inc;
        return time;
    }

    void computePosition()
    {
        setPosition(steps_taken);
    }

    void updateMotorSpeed(double axis_speed)
    {
        curr_dir = axis_speed > 0 ? 0 : 1;
        // digitalWrite(dir_pin, curr_dir);
        stopped = (axis_speed == 0);
        step_delay = (1000L / fabs(axis_speed));
    }

    void pollMotor()
    {
        if (!stopped)
        {
            if ((time - last_step) >= step_delay)
            {
                steps_taken += curr_dir ? -1 : 1;
                last_step = time;
            }
        }
        cout << "#" << endl;
        cout << targetPosition() << ", ";
        cout << currentPosition() << ", ";
        cout << getCMDSpeed() << ", ";
        cout << getCMDAccel() << ", ";
        cout << getCMDJerk() << ", ";
        cout << distanceToStop();
        cout << endl;
    }
};

int main(int argc, char *argv[])
{
    TestAxis axis;

    axis.setTargetSpeed(10);
    axis.setTargetAcceleration(2);
    axis.setTargetJerk(.5);
    axis.setResolution(0);

    for (int i = 0; i < 3000; i++)
    {
        axis.run();
    }

    for (int mode = 0; mode < 2; mode++)
    {
        axis.setLimitMode(mode);

        axis.moveTo(50);
        axis.runToPosition();
        for (int i = 0; i < 3000; i++)
        {
            axis.run();
        }

        axis.moveTo(-25);
        axis.runToPosition();
        for (int i = 0; i < 3000; i++)
        {
            axis.run();
        }

        axis.moveTo(0);
        axis.runToPosition();
        for (int i = 0; i < 3000; i++)
        {
            axis.run();
        }
    }
    return 0;
}