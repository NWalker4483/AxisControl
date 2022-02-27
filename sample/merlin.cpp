#include "multi_axis.h"
#include <Eigen/Dense>

using namespace Eigen;

class MerlinMR6200 : public MultiAxis<6>
{
private:
    Matrix<double, 6, 6> reductions;
    Matrix<double, 6, 6> reductions_inv;
    /* data */
    unsigned int getTime() override
    {
        return 0;
    }

    void computeAxisPositions(double *axis_positions) override
    {
    }

    void updateMotorSpeeds(double *axis_speeds) override
    {
        Matrix<double, 6, 1> speed_holder;
        for(int i = 0; i < 6; i++){
speed_holder(i) = axis_speeds[i];
        }
    }

public:
    MerlinMR6200(/* args */)
    {
        reductions_inv = reductions.inverse();
    }
    ~MerlinMR6200() {}
};
