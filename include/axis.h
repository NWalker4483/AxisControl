
#ifndef Single_Axis_h
#define Single_Axis_h
#include <math.h> /* fabs */
class Axis
{
public:
  Axis(){
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

  void setPosition(double pose);
protected:
  virtual unsigned int getTime() = 0;
  virtual void computePosition() = 0;
  virtual void updateMotorSpeed(double axis_speed) = 0;
  virtual void pollMotor() {};

  void setSpeed(double speed);
  void setAcceleration(double acceleration);

  double min_resolution;

  double real_pose;
  double real_speed;
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