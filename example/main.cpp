#include "multi_axis.h"
#include "iostream"
#include<unistd.h>
using namespace std;
const int text_axis = 1; 
class TestBot: public MultiAxis<text_axis>
{
    public: 
        int t;
        unsigned int inc = 1000;
        unsigned int time;

        TestBot(){
            // Open Log
        }

        ~TestBot(){
            // Close Log
        }
    
    protected:
        unsigned int getMicros() {
            time += inc;
            return time;
        }

        void computeAxisPositions(double *axis_position) {
            for (int i = 0; i < text_axis; i++){
                *(axis_position + i) += axis[i].getCMDSpeed() * ((float)inc/1000.);
            }
        }

        void updateMotorSpeeds(double *axis_speeds) {
           // Nothin
        }

        void pollMotors(){
            cout << "#" << endl;
            for (int i = 0; i < text_axis; i++){
            cout << axis[i].currentPosition() << " ";
            cout << axis[i].getSpeed() << " ";
            // cout << axis[i].getAcceleration() << " ";
            // cout << axis[i].getJerk() << " ";
            cout << endl;
            }
        unsigned int microsecond = 1000000;
        usleep(1 * microsecond);// sleeps for 1 second
        }
};

int main(int argc, char *argv[]){
    TestBot robot;
    robot.setLimitMode(0);
    double moves[text_axis];
    for (int i = 0; i < text_axis; i++){
    robot.axis[i].setTargetSpeed(1);
    moves[i] = -5;
    }
    robot.moveAll(moves);
    robot.runToPositions();
    return 0; 
}