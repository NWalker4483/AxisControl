#include "axis.h"
#include "iostream"
//  Max Interval

template <typename num>
inline num constrain(num val, num min, num max)
{
  if (val < min)
    return min;
  if (val > max)
    return max;
  return val;
}

template <typename num>
num approach(num curr, num min, num max, num step)
{
  /*
  curr should be added to step but with the caveat that it
  cannot decrease below min and cannot increase above max
  Decreasing to a value above max will not cause any constraints and vice versa for increasing to a value below minimum
  */
  if (step > 0)
  {
    return constrain(curr + step, curr, max);
  }
  else
  {
    return constrain(curr + step, min, curr);
  }
}

template <typename num>
inline num sign(num curr)
{
  return curr >= 0 ? 1L : -1L;
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
          true)//(fabs(real_accel) <= target_jerk))
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

  switch (limit_mode)
  {
  case 2:
  if (0){

  }
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
    // Time till speed limit reached
    // double t_al = ((flipped * real_accel) + target_accel) / -target_jerk; // GOOD
    // Time till speed of 0 reached
    double t_jl = -((flipped * real_accel) + sqrt(pow(real_accel, 2) - (2L * flipped * real_speed * -target_jerk))) / -target_jerk; // GOOD

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
