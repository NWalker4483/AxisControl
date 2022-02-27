#include "multi_axis.h"
// TODO: Use templates

template<int axis_cnt>
void MultiAxis<axis_cnt>::moveAllTo(double *absolute)
{
  for (int i = 0; i < axis_cnt; i++)
    axis[i].moveTo(absolute[i]);
}

template<int axis_cnt>
void MultiAxis<axis_cnt>::moveAll(double *relative)
{
  for (int i = 0; i < axis_cnt; i++)
    axis[i].move(relative[i]);
}

template<int axis_cnt>
bool MultiAxis<axis_cnt>::run()
{
  unsigned int curr_time = getTime();
  unsigned int time_passed = curr_time - last_time;
  double positions[axis_cnt];
  computeAxisPositions(positions);

  bool any_speed_changed = false;
  double current_speeds[axis_cnt];

  for (int i = 0; i < axis_cnt; i++)
  {
    axis[i].setPosition(positions[i]);
    axis[i].computeMotionFeatures(time_passed);
    bool speed_changed = axis[i].computeMotionControls(time_passed); // Consider Returning bool
    current_speeds[i] = axis[i].getSpeed();
    any_speed_changed = any_speed_changed or speed_changed;
  }
  if (any_speed_changed)
  {
    updateMotorSpeeds(current_speeds);
  }

  pollMotors();
  last_time = curr_time;

  bool done = true;
  for (int i = 0; i < axis_cnt; i++) done = done and (current_speeds[i] == 0);

  return not done;
}

template<int axis_cnt>
void MultiAxis<axis_cnt>::runToPositions()
{
  while (run())
    ;
}

template<int axis_cnt>
void MultiAxis<axis_cnt>::stop()
{
    for (int i = 0; i < axis_cnt; i++) axis[i].stop();
}
