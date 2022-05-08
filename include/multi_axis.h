#ifndef Multi_Axis_h
#define Multi_Axis_h
#include "axis.h"

template <size_t axis_cnt>
class MultiAxis
{
public:
  MultiAxis<axis_cnt>()
  {
    for (int i = 0; i < axis_cnt; i++)
    {
      current_positions[i] = 0;
    }
  }
  ~MultiAxis<axis_cnt>() {}
  Axis axis[axis_cnt];

  void setTargetJerk(double value);
  void setTargetAcceleration(double value);
  void setTargetSpeed(double value);
  void setLimitMode(int mode);

  void moveAllTo(double (&absolute)[axis_cnt]);
  void moveAll(double (&relative)[axis_cnt]);

  bool run();
  void runToPositions();
  void stop();

protected:
  double current_positions[axis_cnt];
  double current_speeds[axis_cnt];

  virtual unsigned int getMillis() = 0;
  virtual void computeAxisPositions(double *axis_positions) = 0;
  virtual void updateMotorSpeeds(double *axis_speeds) = 0;
  virtual void pollMotors(){};

};

template <size_t axis_cnt>
void MultiAxis<axis_cnt>::setLimitMode(int mode)
{
  for (int i = 0; i < axis_cnt; i++)
    axis[i].setLimitMode(mode);
}

template <size_t axis_cnt>
void MultiAxis<axis_cnt>::setTargetJerk(double mode)
{
  for (int i = 0; i < axis_cnt; i++)
    axis[i].setTargetJerk(mode);
}

template <size_t axis_cnt>
void MultiAxis<axis_cnt>::setTargetAcceleration(double mode)
{
  for (int i = 0; i < axis_cnt; i++)
    axis[i].setTargetAcceleration(mode);
}

template <size_t axis_cnt>
void MultiAxis<axis_cnt>::setTargetSpeed(double mode)
{
  for (int i = 0; i < axis_cnt; i++)
    axis[i].setTargetSpeed(mode);
}

template <size_t axis_cnt>
void MultiAxis<axis_cnt>::stop()
{
  for (int i = 0; i < axis_cnt; i++)
    axis[i].stop();
}

template <size_t axis_cnt>
void MultiAxis<axis_cnt>::runToPositions()
{
  while (not run())
    ;
}

template <size_t axis_cnt>
void MultiAxis<axis_cnt>::moveAllTo(double (&absolute)[axis_cnt])
{
  for (int i = 0; i < axis_cnt; i++)
    axis[i].moveTo(absolute[i]);
}

template <size_t axis_cnt>
void MultiAxis<axis_cnt>::moveAll(double (&relative)[axis_cnt])
{
  for (int i = 0; i < axis_cnt; i++)
    axis[i].move(relative[i]);
}

/*run*/
template <size_t axis_cnt>
bool MultiAxis<axis_cnt>::run()
{
  unsigned int curr_time = getMillis();

  computeAxisPositions(current_positions);

  bool any_speed_changed = false;

  double old_speed, new_speed;

  for (int i = 0; i < axis_cnt; i++)
  {
    axis[i].setPosition(current_positions[i]);

    old_speed = axis[i].getCMDSpeed();
    axis[i].run(curr_time);
    new_speed = axis[i].getCMDSpeed();

    any_speed_changed = any_speed_changed or (new_speed != old_speed);
    current_speeds[i] = new_speed;
  }
  if (any_speed_changed)
  {
    updateMotorSpeeds(current_speeds);
  }

  pollMotors();

  bool done = true;
  for (int i = 0; i < axis_cnt; i++)
    done = done and axis[i].isDone();

  return done;
}
#endif
// https://www.baslerweb.com/fp-1636374975/media/downloads/software/pylon_software/pylon_6.3.0.23157_x86_64_setup.tar.gz