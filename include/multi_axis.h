#ifndef Multi_Axis_h
#define Multi_Axis_h
#include "axis.h"

template <size_t axis_cnt>
class MultiAxis
{
public:
  MultiAxis<axis_cnt>() {
  for (int i = 0; i < axis_cnt; i++){
    current_positions[i] = 0;
  }
  }
  ~MultiAxis<axis_cnt>() {}
  Axis axis[axis_cnt];

  void setLimitMode(int mode);

  void moveAllTo(double (&absolute)[axis_cnt]);
  void moveAll(double (&relative)[axis_cnt]);

  bool run();
  void runToPositions();
  void stop();

protected:
  double current_positions[axis_cnt];
  double current_speeds[axis_cnt];

  virtual unsigned int getMicros() = 0;
  virtual void computeAxisPositions(double *axis_positions) = 0;
  virtual void updateMotorSpeeds(double *axis_speeds) = 0;
  virtual void pollMotors(){};

private:
  unsigned int last_time;
};

template <size_t axis_cnt>
 void MultiAxis<axis_cnt>::setLimitMode(int mode)
{
  for (int i = 0; i < axis_cnt; i++)
    axis[i].setLimitMode(mode);
}

template <size_t axis_cnt>
 void MultiAxis<axis_cnt>::stop()
{
  for (int i = 0; i < axis_cnt; i++)
    axis[i].stop();
}

template <size_t axis_cnt>
 void MultiAxis<axis_cnt>::runToPositions()
{
  while (run())
    ;
}

template <size_t axis_cnt>
 void MultiAxis<axis_cnt>::moveAllTo(double (&absolute)[axis_cnt])
{
  for (int i = 0; i < axis_cnt; i++)
    axis[i].moveTo(absolute[i]);
}

template <size_t axis_cnt>
 void MultiAxis<axis_cnt>::moveAll(double (&relative)[axis_cnt])
  {
    for (int i = 0; i < axis_cnt; i++)
      axis[i].move(relative[i]);
  }

///////////////////////////////////////
template<size_t axis_cnt>
bool MultiAxis<axis_cnt>::run()
{
  unsigned int curr_time = getMicros();
  unsigned int time_passed = curr_time - last_time;
  
  computeAxisPositions(current_positions);

  bool any_speed_changed = false;

  for (int i = 0; i < axis_cnt; i++)
  {
    axis[i].setPosition(current_positions[i]);
    //axis[i].computeMotionFeatures(time_passed);
    bool speed_changed = axis[i].computeMotionControls(time_passed); // Consider Returning bool
    current_speeds[i] = axis[i].getCMDSpeed();
    any_speed_changed = any_speed_changed or speed_changed;
  }
  if (any_speed_changed)
  {
    updateMotorSpeeds(current_speeds);
  } 

  pollMotors();
  last_time = curr_time;

  bool done = true;
  for (int i = 0; i < axis_cnt; i++) done = done and (fabs(axis[i].distanceToGo()) < axis[i].min_resolution);

  return not done;
}
#endif
