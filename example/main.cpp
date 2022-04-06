#include "multi_axis.h"
#include "iostream"
const int text_axis = 2; 
class TestBot: public MultiAxis<text_axis>
{
    public: 
        int t;
        unsigned int inc = 10;
        unsigned int time;
        double poses[text_axis];

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
                axis[i].getSpeed();
                axis[i].getAcceleration();
                std::cout << text_axis;
            }
        }

        void updateMotorSpeeds(double *axis_speeds) {
            // Log Data
            std::cout << "1";
        }
};

int main(int argc, char *argv[]){
    TestBot robot;
    robot.setLimitMode(1);
    double moves[2] = {25,25};
    robot.moveAll(moves);
    robot.run();
    robot.stop();
    return 0; 
}