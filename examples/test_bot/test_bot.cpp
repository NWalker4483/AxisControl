#include "multi_axis.h"
#include "iostream"
#include <unistd.h>
using namespace std;
const int text_axis = 1;
class TestBot : public MultiAxis<text_axis>
{
public:
    int t;
    unsigned int inc = 1;
    unsigned int time = 0;

    TestBot()
    {
    }
w
    ~TestBot()
    {
    }

protected:
    unsigned int getMillis()
    {
        time += inc;
        return time;
    }

    void computeAxisPositions(double *axis_position)
    {

        for (int i = 0; i < text_axis; i++)
        {
            *(axis_position + i) = axis[i].currentPosition() + (axis[i].getCMDSpeed() * ((double)inc / 1000L));
        }
    }

    void updateMotorSpeeds(double *axis_speeds)
    {
        // Nothin
    }

    void pollMotors()
    {
        cout << "#" << endl;
        for (int i = 0; i < text_axis; i++)
        {
            cout << axis[i].targetPosition() << ", ";
            cout << axis[i].currentPosition() << ", ";
            cout << axis[i].getCMDSpeed() << ", ";
            cout << axis[i].getCMDAccel() << ", ";
            cout << axis[i].getCMDJerk() << ", ";
            cout << axis[i].distanceToStop();
            cout << endl;
        }
    }
};

int main(int argc, char *argv[])
{
    TestBot robot;
    for (int i = 0; i < text_axis; i++)
    {
        robot.axis[i].setTargetSpeed(8);
        robot.axis[i].setTargetAcceleration(4);
        robot.axis[i].setTargetJerk(.5);
    }
     for (int i = 0; i < 3000; i++)
    {
        robot.run();
    }
    double moves[text_axis];
    for (int mode = 2; mode < 3; mode++){
    robot.setLimitMode(mode);

    for (int i = 0; i < text_axis; i++) moves[i] = 25;
    robot.moveAllTo(moves);
    robot.runToPositions();
    for (int i = 0; i < 3000; i++)
    {
        robot.run();
    }
    for (int i = 0; i < text_axis; i++) moves[i] = -25;
    robot.moveAllTo(moves);
    robot.runToPositions();
    for (int i = 0; i < 3000; i++)
    {
        robot.run();
    }
    for (int i = 0; i < text_axis; i++) moves[i] = 0;
    robot.moveAllTo(moves);
    robot.runToPositions();
    for (int i = 0; i < 3000; i++)
    {
        robot.run();
    }}
    return 0;
}