#include "axis.h"
template <typename num>
num constrain(num val, num min, num max)
{
  if (val < min)
    return min;
  if (val > max)
    return max;
  return val;
}

// TODO: use template
double approach(double curr, double min, double max, double step)
{
  // curr should be added to step but with the caveat that it
  // cannot decrease below min and cannot increase above max
  if (step > 0)
  {
    return constrain(curr + step, curr, max);
  }
  else
  {
    return constrain(curr + step, min, curr);
  }
}

bool Axis::computeMotionControls(unsigned int time_passed)
{
  double distance_left = distanceToGo();
  double dist_to_stop= distanceToStop();
  if ((fabs(distance_left) < min_resolution) or (distance_left == 0)) // Need to stop
  {
    setSpeed(0);
  }
  else
  {
    switch (limit_mode)
    {
    case 2:
      setJerk(distance_left > dist_to_stop ? -target_jerk : target_jerk);
      setAcceleration(approach(real_accel, -target_accel, target_accel, (cmd_jerk / (time_passed/1000.))));
      setSpeed(approach(real_speed, -target_speed, target_speed, (cmd_accel / (time_passed/1000.))));
      break;
    case 1:
      setAcceleration(distance_left > dist_to_stop ? target_accel : -target_accel);
      setSpeed(approach(real_speed, -target_speed, target_speed, (cmd_accel / (time_passed/1000.))));
      break;
    case 0:
      setSpeed(distance_left > 0 ? -target_speed : target_speed);
      break;
    }
  }
  return _speed_changed;
}

void Axis::computeMotionFeatures(unsigned int time_passed)
{
  last_speed = real_speed;
  last_accel = cmd_accel;

  double d_p = last_pose - real_pose;
  real_speed = d_p / (time_passed/1000.);
  double d_v = last_speed - real_speed;
  real_accel = d_v / (time_passed/1000.);
}

double Axis::distanceToStop()
{
  switch (limit_mode)
  {
  case 0:
    return 0;
    break;

  case 1:
    return fabs((real_speed * real_speed) / (2. * real_accel));
    break;

  case 2: // Jerk Control Not Implemented
    return 0;
    break;

  default:
    return 0;
  }
}

bool Axis::run()
{
  unsigned int curr_time = getTime();
  unsigned int time_passed = curr_time - last_time;

  computePosition();
  computeMotionFeatures(time_passed);
  computeMotionControls(time_passed);
  if (_speed_changed)
  {
    updateMotorSpeed(cmd_speed); // Should take in
    _speed_changed = false;
  }
  pollMotor();
  last_time = curr_time;

  bool done = cmd_speed == 0;
  return not done;
}

void Axis::stop()
{
  if (getSpeed() > 0)
  {
    move(distanceToStop());
  }
  else
  {
    move(-distanceToStop());
  }
}

void Axis::setPosition(double pose)
//TODO: Combine with computeMotionFeatures function
{
  last_pose = real_pose;
  real_pose = pose;
}
