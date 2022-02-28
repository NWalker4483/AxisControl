
#ifndef Single_Axis_h
#define Single_Axis_h
#include <math.h> /* fabs */
#include "utils.h"
class Axis
{
public:
  Axis();

  void moveTo(double absolute);
  void move(double relative);

  void setMaxSpeed(double speed);
  void setMaxAcceleration(double acceleration);
  void setMaxJerk(double jerk);

  void setLimitMode(int mode){}

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

  void setPosition(double pose);
protected:
  virtual unsigned int getTime();
  virtual void computePosition();
  virtual void updateMotorSpeed(double axis_speed);
  virtual void pollMotor() {};

  void setSpeed(double speed);
  void setAcceleration(double acceleration);

  double min_resolution = 1;

  double real_pose;
  double _speed;
  double _accel;

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

  double max_jerk;

  int limit_mode;

  unsigned int last_time;

  bool _target_changed;
  bool _speed_changed; 
  bool _accel_changed;
};

#endif