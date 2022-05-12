
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

bool Axis::computeMotionControls(int time_passed)
{
  double seconds_passed = 1000L / time_passed;
  double distance_left = distanceToGo();
  double dist_to_stop = distanceToStop();
  bool approaching_target = sign(dist_to_stop) == sign(distance_left);
  bool slowdown = (fabs(distance_left) > fabs(dist_to_stop)) or !approaching_target;
  double accel_dir = currentPosition() > targetPosition() ? 1L : -1L; // Direction of acceleration towards target
  if ((fabs(distance_left) < min_resolution) or (distance_left == 0)) // Need to stop
  {
    switch (limit_mode)
    {
    case 0:
      setSpeed(0);
      is_done = true;
      return true;
      break;
    case 1:
      if (pow(real_speed,2) <= fabs(target_accel))
      {
        setSpeed(0);
        setAcceleration(0);
        is_done = true;
        return true;
      }
      break;
    case 2:
      if ((pow(real_speed, 2) <= target_accel) and
        (fabs(real_accel) <= target_jerk))
      {
        setSpeed(0);
        setAcceleration(0);
        setJerk(0);
        is_done = true;
        return true;
      }
      break;
    }
    slowdown = true;
  }
  is_done = false;

double flipped = real_speed < 0 ? -1L : 1L; // GOOD
double t_a0;double d_a0;
 double stop_distance;
  switch (limit_mode)
  {
  case 2:
  
  t_a0 = (fabs(real_accel) / target_jerk);
  d_a0 = 0;

  if ((3 * d_a0) > fabs(dist_to_stop)) {slowdown = false;}

  if (fabs(d_a0) >= fabs(distance_left)) {
    // Accel Should Approach 0 in whatever neccesary direction 
    
  }

if fabs d_a0 < fabs distance_left and dist_to_stop > distance_left
    setJerk(accel_dir * (slowdown ? -target_jerk : target_jerk));
    setAcceleration(approach(real_accel, -target_accel, target_accel, (cmd_jerk / seconds_passed)));


    setSpeed(approach(real_speed, -target_speed, target_speed, (cmd_accel / seconds_passed)));
    break;
  case 1:
    setAcceleration(accel_dir * (slowdown ? -target_accel : target_accel));
    setSpeed(approach(real_speed, -target_speed, target_speed, cmd_accel / seconds_passed));
    break;
  case 0:
    setSpeed(distance_left < 0 ? -target_speed : target_speed);
    break;
  }

  return _speed_changed;
}

void Axis::computeMotionFeatures(int time_passed)
{
  last_speed = real_speed;
  last_accel = real_accel;
  last_jerk = real_jerk;

  double seconds_passed = (1000L / time_passed);

  double d_p = real_pose - last_pose;
  real_speed = d_p * seconds_passed;
  last_pose = real_pose;

  double d_v = real_speed - last_speed;
  real_accel = d_v * seconds_passed;

  double d_a = real_accel - last_accel;
  real_jerk = d_a * seconds_passed;
}

double Axis::distanceToStop()
{
  switch (limit_mode)
  {
  case 0:
    return 0;
    break;

  case 1:
    return ((pow(real_speed, 2) / (2L * target_accel)) + 0 ) * sign(real_speed);
    break;

  case 2:
    // 
    double distance;
    double flipped = real_speed < 0 ? -1L : 1L; // GOOD
    // Time till accel limit reached
    double t_al = ((flipped * real_accel) + target_accel) / -target_jerk; // GOOD
    double t_a0 = fabs(real_accel)/ target_jerk;
    // Time till speed limit reached
    // double t_al = ((flipped * real_accel) + target_accel) / -target_jerk; // GOOD
    // Time till speed of 0 reached
    double t_jl = -((flipped * real_accel) + sqrt(pow(real_accel, 2) - (2L * flipped * real_speed * -target_jerk))) / -target_jerk; // GOOD

    // t_jl = (t_jl > t_a0) ? t_jl : t_a0;
    // if (t_al < t_jl)
    // { // Compute stopping distance in 2 parts
    //   double p_al = (real_speed * t_al * flipped) + ((flipped * real_accel * pow(t_al, 2))/2) + ((-target_jerk * pow(t_al, 3)) / 6);// GOOD
    //   double v_al = (real_speed * flipped) + (real_accel * flipped * t_al) + ((-target_jerk * pow(t_al, 2))/2); // GOOD
      
    //   distance = p_al + (pow(v_al,2) / (2L * target_accel)); // GOOD
    // }
    // else
    // { // Compute in single part
      distance = (flipped * real_speed * t_jl) + ((flipped * real_accel * pow(t_jl, 2))/2) + ((-target_jerk * pow(t_jl, 3)) / 6);
    // }
    return distance * flipped;
    break;
  }
  return 0;
}

bool Axis::run()
{
  return run(getMillis());
}

bool Axis::run(unsigned int curr_time)
{
  int time_passed = curr_time - last_time;
  computePosition();

  if (_pose_changed or (time_passed >= 50)) // Limit time between calculations for incremental motors
  {
    computeMotionFeatures(time_passed);
    computeMotionControls(time_passed);
    _pose_changed = false;
    last_time = last_time + time_passed;
  }
 
  if (cmd_speed == 0)
    computeMotionControls(time_passed); // Double Check Blah Blah

  if (_speed_changed)
  {
    updateMotorSpeed(cmd_speed); // Should take in -\(`_`)/-
    _speed_changed = false;
  }

  pollMotor();

  return isDone();
}

#endif