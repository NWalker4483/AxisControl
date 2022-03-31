#include "multi_axis.h"

class TestBot: public MultiAxis<2>
{
    public: 
        int t = 5;
        unsigned int time;

    TestBot(){

    }

    ~TestBot(){

    }
    
    protected:
        unsigned int getTime() {
            time += 100;
            return time;
        }

        void computeAxisPositions(double *axis_position) {
            for (int i = 0; i < 2; i++){}
        }

        void updateMotorSpeeds(double *axis_speeds) {
            // Log Data
        }
};

int main(int argc, char *argv[]){
    TestBot robot;
    robot.setLimitMode(1);
    return 0; 
}