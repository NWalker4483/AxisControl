
#ifndef Single_Axis_h
#define Single_Axis_h
#include <math.h> /* fabs */
class Axis
{
public:
  Axis()
  {
    min_resolution = 1;
  };

  void moveTo(double absolute);
  void move(double relative);

  void setTargetSpeed(double speed);
  void setTargetAcceleration(double acceleration);
  void setTargetJerk(double jerk);

  void setResolution(double res);
  void setLimitMode(int mode);

  bool computeMotionControls(unsigned int time_passed);
  void computeMotionFeatures(unsigned int time_passed);

  double distanceToGo();
  double distanceToStop();

  double targetPosition();
  double currentPosition();

  bool run();
  void runToPosition();

  void stop();
  double getSpeed();
  double getAcceleration();
  double getJerk(){return 1;};

  void setPosition(double pose);

protected:
  virtual unsigned int getTime(){return 0;};
  virtual void computePosition(){};
  virtual void updateMotorSpeed(double axis_speed){};
  virtual void pollMotor(){};

  void setSpeed(double speed);
  void setAcceleration(double acceleration);
  void setJerk(double jerk);

  double real_pose;
  double real_speed;
  double real_accel;

private:
  double target_pose;
  // double cmd_pose;
  double last_pose;

  double target_speed;
  double cmd_speed;
  double last_speed;

  double target_accel;
  double cmd_accel;
  double last_accel;

  double target_jerk;
  double cmd_jerk;
  // double last_jerk;

  int limit_mode;
  double min_resolution;

  unsigned int last_time;

  bool _target_changed;
  bool _speed_changed;
  bool _accel_changed;
};

inline void Axis::moveTo(double absolute)
{
  if (target_pose != absolute)
  {
    _target_changed = true;
    target_pose = absolute;
  }
}

inline void Axis::setSpeed(double speed)
{
  if (cmd_speed != speed)
  {
    cmd_speed = speed;
    _speed_changed = true;
  }
}

inline void Axis::setAcceleration(double acceleration)
{
  if (cmd_accel != acceleration)
  {
    cmd_accel = acceleration;
    _accel_changed = true;
  }
}

inline void Axis::setJerk(double jerk)
{
  if (cmd_jerk != jerk)
  {
    cmd_jerk = jerk;
    // _jerk_changed = true;
  }
}

inline void Axis::move(double relative)
{
  moveTo((real_pose + relative));
}

inline void Axis::setTargetSpeed(double speed)
{
  speed = fabs(speed);
  target_speed = speed;
}

inline void Axis::setTargetAcceleration(double acceleration)
{
  acceleration = fabs(acceleration);
  target_accel = acceleration;
}

inline void Axis::setTargetJerk(double jerk)
{
  jerk = fabs(jerk);
  target_jerk = jerk;
}

inline double Axis::getSpeed()
{
  return real_speed;
}

inline double Axis::getAcceleration()
{
  return cmd_accel;
}

inline void Axis::setLimitMode(int mode)
{
  limit_mode = mode;
}

inline double Axis::targetPosition()
{
  return target_pose;
}

inline double Axis::currentPosition()
{
  return real_pose;
}

inline void Axis::runToPosition()
{
  while (run())
    ;
}

inline double Axis::distanceToGo()
{
  return target_pose - real_pose;
}
#endif