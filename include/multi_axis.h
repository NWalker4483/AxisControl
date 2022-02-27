#ifndef Multi_Axis_h
#define Multi_Axis_h
#include "axis.h"
#include <math.h> /* fabs */

template<int axis_cnt>
class MultiAxis
{
public:
  MultiAxis<axis_cnt>() {
  }
  ~MultiAxis<axis_cnt>() {}
  Axis axis[axis_cnt];

  void setLimitMode(int mode){}
  void moveAllTo(double *absolute);
  void moveAll(double *relative);

  bool run();
  void runToPositions();
  void stop();

protected:
  virtual unsigned int getTime() = 0;
  virtual void computeAxisPositions(double *axis_positions) = 0;
  virtual void updateMotorSpeeds(double *axis_speeds) = 0;
  virtual void pollMotors() {};

private:
  unsigned int last_time;
};
#endif
