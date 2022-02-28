#include "axis.h"

Axis::Axis() {}

void Axis::computePosition() {}
void Axis::updateMotorSpeed(double axis_speed) {}
unsigned int Axis::getTime() { return 0; }

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

void Axis::setMaxSpeed(double speed)
{
  speed = fabs(speed);
  target_speed = speed;
}
void Axis::setMaxAcceleration(double acceleration)
{
  acceleration = fabs(acceleration);
  target_accel = acceleration;
}
void Axis::setMaxJerk(double jerk)
{
  jerk = fabs(jerk);
  max_jerk = jerk;
}

double Axis::getSpeed()
{
  return _speed;
}
double Axis::getAcceleration()
{
  return _accel;
}
bool Axis::computeMotionControls(unsigned int time_passed)
{
  double distance_left = distanceToGo();

  if ((fabs(distance_left) < min_resolution) or (distance_left == 0)) // Need to stop
  {
    setSpeed(0);
  }
  else
  {
    switch(limit_mode){
    case 2: // Not Implemented
      setAcceleration(_accel + (_accel/time_passed));
      break;
    case 1:
    double dist_to_stop = distanceToStop();
    setAcceleration(distance_left > dist_to_stop ? target_accel : -target_accel);
    if (getAcceleration() > 0){ // Speeding Up
      setSpeed(constrain(_speed + (_accel / time_passed), _speed, target_speed));
      }
    else { // Slowing Down
      setSpeed(constrain(_speed - (_accel / time_passed), -target_speed, _speed));
    }
    break;
    case 0:
      setSpeed(distance_left > 0 ? target_speed :-target_speed);
      break;
    } 
  }
  return _speed_changed;
}
void Axis::computeMotionFeatures(unsigned int time_passed)
{
  last_speed = _speed;
  last_accel = _accel;

  double d_p = last_pose - real_pose;
  // TODO: Correct Units
  _speed = d_p / time_passed;
  double d_v = last_speed - _speed;
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
    return fabs((_speed * _speed) / (2. * _accel));
    break;
  case 2:
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
    updateMotorSpeed(_speed); // Should take in
    _speed_changed = false;
  }
  pollMotor();
  last_time = curr_time;

  bool done = _speed == 0;
  return not done;
}

void Axis::runToPosition()
{
  while (run())
    ;
}
void Axis::stop()
{
  // TODO Account for direction
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
  if (_speed != speed)
  {
    _speed = speed;
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
