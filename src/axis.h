
#ifndef Single_Axis_h
#define Single_Axis_h
#include <math.h> /* fabs */
#include "helpers.h"

class Axis
{
public:
  Axis()
  {
    min_resolution = .0125;
  };

  void moveTo(double absolute);
  void move(double relative) { moveTo((real_pose + relative)); };

  void setTargetSpeed(double speed) { target_speed = fabs(speed); };
  void setTargetAcceleration(double acceleration) { target_accel = fabs(acceleration); };
  void setTargetJerk(double jerk) { target_jerk = fabs(jerk); };

  void setResolution(double res) { min_resolution = fabs(res); };
  void setLimitMode(int mode) { limit_mode = mode; };

  bool computeMotionControls(int time_passed);
  void computeMotionFeatures(int time_passed);

  double distanceToGo() { return target_pose - real_pose; };
  double distanceToStop();

  double targetPosition() { return target_pose; };
  double currentPosition() { return real_pose; };

  void setLimits(double upper, double lower){};
  void clearLimits(){};

  bool run();
  bool run(unsigned int curr_time);
  void runToPosition();
  bool isDone() { return is_done; };

  void stop() { move(distanceToStop()); };

  void setPosition(double position);

  double getSpeed() { return real_speed; };
  double getAcceleration() { return real_accel; };
  double getJerk() { return real_jerk; };

  double getTargetSpeed() { return target_speed; };
  double getTargetAcceleration() { return target_accel; };
  double getTargetJerk() { return target_jerk; };

  double getCMDAccel() { return cmd_accel; };
  double getCMDSpeed() { return cmd_speed; };
  double getCMDJerk() { return cmd_jerk; };

protected:
  virtual unsigned int getMillis() { return 0; };
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

  double min_resolution;

private:
  double target_pose = 0;
  double cmd_pose = 0; // TODO: Use in forced interval mode
  double last_pose = 0;

  double target_speed = 0;
  double cmd_speed = 0;
  double last_speed = 0;

  double target_accel = 0;
  double cmd_accel = 0;
  double last_accel = 0;

  double target_jerk = 0;
  double cmd_jerk = 0;
  double last_jerk = 0;

  int limit_mode = 0;
  bool is_done = true;

  unsigned int last_time = 0;

  bool _target_changed = false; // ? I dont think I need this variable anymore, since the motioncontrols function checks the direction whenever its called
  bool _pose_changed = false;
  bool _speed_changed = false;
  bool _accel_changed = false;
  bool _jerk_changed = false;
};

void Axis::moveTo(double absolute)
{
  if (target_pose != absolute)
  {
    _target_changed = true;
    target_pose = absolute;
  }
}

void Axis::setPosition(double pose)
{
  if (real_pose != pose)
  {
    real_pose = pose;
    _pose_changed = true;
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
    _jerk_changed = true;
  }
}

void Axis::runToPosition()
{
  while (!run())
    ;
}

#endif