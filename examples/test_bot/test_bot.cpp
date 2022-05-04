#include "multi_axis.h"
#include "iostream"
#include <unistd.h>
using namespace std;
const int text_axis = 1;
class TestBot : public MultiAxis<text_axis>
{
public:
    int t;
    unsigned int inc = 10;
    unsigned int time = 0;

    TestBot()
    {
    }

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
            cout << axis[i].currentPosition() << ", ";
            cout << axis[i].getSpeed() << ", ";
            cout << axis[i].getAcceleration() << ", ";
            cout << axis[i].getJerk() << " ";
            cout << endl;
        }
    }
};

int main(int argc, char *argv[])
{
    TestBot robot;
    robot.setLimitMode(1);
    double moves[text_axis];
    for (int i = 0; i < text_axis; i++)
    {
        // Signs of Targets are autocorrected as neccesarry to reach
        robot.axis[i].setTargetSpeed(-5);
        robot.axis[i].setTargetAcceleration(-.5);
        moves[i] = -25;
    }
    robot.moveAll(moves);
    robot.runToPositions();
    for (int i = 0; i < 1000; i++)
    {
        robot.run();
    }

    // for (int i = 0; i < text_axis; i++)
    // {
    //     // Signs of Targets are autocorrected as neccesarry to reach
    //     robot.axis[i].setTargetSpeed(-10);
    //     robot.axis[i].setTargetAcceleration(-.25);
    //     moves[i] = 25;
    // }
    // robot.moveAll(moves);
    // robot.runToPositions();

    // for (int i = 0; i < 1000; i++)
    // {
    //     robot.run();
    // }
    return 0;
}