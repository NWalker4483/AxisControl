
#ifndef Single_Axis_h
#define Single_Axis_h
#include <math.h> /* fabs */
class Axis
{
public:
  Axis()
  {
    min_resolution = 1;
    real_pose = 0;
  };

  void moveTo(double absolute);
  void move(double relative);

  void setTargetSpeed(double speed);
  void setTargetAcceleration(double acceleration);
  void setTargetJerk(double jerk);

  void setResolution(double res);
  void setLimitMode(int mode);

  bool computeMotionControls( int time_passed);
  void computeMotionFeatures( int time_passed);

  double distanceToGo();
  double distanceToStop();

  double targetPosition();
  double currentPosition();

  bool run();
  void runToPosition();

  void stop();
  double getSpeed() {return real_speed;};
  double getAcceleration(){ return real_accel; };
  double getJerk() { return real_jerk; };

  double getTargetSpeed() { return target_speed; };
  double getTargetAcceleration() { return target_accel; };
  double getTargetJerk() { return target_jerk; };

  // For Debugging
  double getCMDAccel() { return cmd_accel;};
  double getCMDSpeed() { return cmd_speed;};
  double getCMDJerk() { return cmd_jerk;};

  void setPosition(double pose);

  double min_resolution;

protected:
  virtual unsigned int getMicros() { return 0; };
  virtual void computePosition(){};
  virtual void updateMotorSpeed(double axis_speed){};
  virtual void pollMotor(){};

  void setSpeed(double speed);
  void setAcceleration(double acceleration);
  void setJerk(double jerk);

  double real_pose = 0;
  double real_speed = 0;
  double real_accel = 0;
  double real_jerk = 0;

private:
  double target_pose = 0;
  // double cmd_pose;
  double last_pose = 0;

  double target_speed = 0;
  double cmd_speed = 0;
  double last_speed = 0;

  double target_accel = 0;
  double cmd_accel = 0;
  double last_accel = 0;

  double target_jerk = 0;
  double cmd_jerk = 0;
  // double last_jerk;

  int limit_mode = 0;

  unsigned int last_time = 0;

  bool _target_changed;
  bool _speed_changed;
  bool _accel_changed;
};

void Axis::moveTo(double absolute)
{
  if (target_pose != absolute)
  {
    _target_changed = true;
    target_pose = absolute;
  }
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
  if (cmd_accel != acceleration)
  {
    cmd_accel = acceleration;
    _accel_changed = true;
  }
}

void Axis::setJerk(double jerk)
{
  if (cmd_jerk != jerk)
  {
    cmd_jerk = jerk;
    // _jerk_changed = true;
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
  target_jerk = jerk;
}

void Axis::setLimitMode(int mode)
{
  limit_mode = mode;
}

double Axis::targetPosition()
{
  return target_pose;
}

double Axis::currentPosition()
{
  return real_pose;
}

void Axis::runToPosition()
{
  while (run())
    ;
}

double Axis::distanceToGo()
{
  return target_pose - real_pose;
}
#endif