#include "axis.h"
template <typename num>
num constrain(num val, num min, num max)
{
  if (val < min)return min;
  if (val > max)return max;
  return val;
}

// TODO: use template 
double approach(double curr, double min, double max, double step){
  // curr should be added to step but with the caveat that it 
  // cannot decrease below min and cannot increase above max
  if (step > 0) {
    return constrain(curr + step, curr, max);
  } else {
 return constrain(curr + step, min, curr);
  }
}

void Axis::moveTo(double absolute)
{
  if (target_pose != absolute)
  {
    _target_changed = true;
    target_pose = absolute;
  }
}

void Axis::move(double relative)
{
  moveTo((real_pose + relative));
}

void Axis::setTargetSpeed(double speed)
{
  speed = fabs(speed);
  target_speed = speed;
}

void Axis::setTargetAcceleration(double acceleration)
{
  acceleration = fabs(acceleration);
  target_accel = acceleration;
}

void Axis::setTargetJerk(double jerk)
{
  jerk = fabs(jerk);
  max_jerk = jerk;
}

double Axis::getSpeed()
{
  return real_speed;
}

double Axis::getAcceleration()
{
  return _accel;
}

bool Axis::computeMotionControls(unsigned int time_passed)
{
  double distance_left = distanceToGo();
  double dist_to_stop;
  if ((fabs(distance_left) < min_resolution) or (distance_left == 0)) // Need to stop
  {
    setSpeed(0);
  }
  else
  {
    switch (limit_mode)
    {
    case 2: // Jerk Control Not Implemented
      setAcceleration(_accel + (_accel / time_passed));
      break;
    case 1:
      dist_to_stop = distanceToStop();
      setAcceleration(distance_left > dist_to_stop ? target_accel : -target_accel);
      setSpeed(approach(real_speed, -target_speed, target_speed, (_accel / time_passed)));
      break;
    case 0:
      setSpeed(distance_left > 0 ? -target_speed : target_speed);
      break;
    }
  }
  return _speed_changed;
}

void Axis::setLimitMode(int mode)
{
  limit_mode = mode;
}

void Axis::computeMotionFeatures(unsigned int time_passed)
{
  last_speed = real_speed;
  last_accel = _accel;

  double d_p = last_pose - real_pose;
  real_speed = d_p / time_passed;
  double d_v = last_speed - real_speed;
  _accel = d_v / time_passed;
}

double Axis::distanceToGo()
{
  return target_pose - real_pose;
}

double Axis::distanceToStop()
{
  switch (limit_mode)
  {
  case 0:
    return 0;
    break;

  case 1:
    return fabs((real_speed * real_speed) / (2. * _accel));
    break;

  case 2: // Jerk Control Not Implemented
    return 0;
    break;

  default:
    return 0;
  }
}

double Axis::targetPosition()
{
  return target_pose;
}

double Axis::currentPosition()
{
  return real_pose;
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

void Axis::runToPosition()
{
  while (run());
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
{
  last_pose = real_pose;
  real_pose = pose;
}

void Axis::setSpeed(double speed)
{
  if (cmd_speed != speed)
  {
    cmd_speed = speed;
    _speed_changed = true;
  }
}
void Axis::setAcceleration(double acceleration)
{
  if (_accel != acceleration)
  {
    _accel = acceleration;
    _accel_changed = true;
  }
}
