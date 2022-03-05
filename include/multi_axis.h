#ifndef Multi_Axis_h
#define Multi_Axis_h
#include "axis.h"

template <size_t axis_cnt>
class MultiAxis
{
public:
  MultiAxis<axis_cnt>() {}
  ~MultiAxis<axis_cnt>() {}
  Axis axis[axis_cnt];

  void setLimitMode(int mode);

  void moveAllTo(double (&absolute)[axis_cnt]);
  void moveAll(double (&relative)[axis_cnt]);

  bool run();
  void runToPositions();
  void stop();

protected:
  double current_positions[axis_cnt];
  double current_speeds[axis_cnt];

  virtual unsigned int getTime() = 0;
  virtual void computeAxisPositions(double *axis_positions) = 0;
  virtual void updateMotorSpeeds(double *axis_speeds) = 0;
  virtual void pollMotors(){};

private:
  unsigned int last_time;
};

template <size_t axis_cnt>
inline void MultiAxis<axis_cnt>::setLimitMode(int mode)
{
  for (int i = 0; i < axis_cnt; i++)
    axis[i].setLimitMode(mode);
}

template <size_t axis_cnt>
inline void MultiAxis<axis_cnt>::stop()
{
  for (int i = 0; i < axis_cnt; i++)
    axis[i].stop();
}

template <size_t axis_cnt>
inline void MultiAxis<axis_cnt>::runToPositions()
{
  while (run())
    ;
}

template <size_t axis_cnt>
inline void MultiAxis<axis_cnt>::moveAllTo(double (&absolute)[axis_cnt])
{
  for (int i = 0; i < axis_cnt; i++)
    axis[i].moveTo(absolute[i]);
}

template <size_t axis_cnt>
inline void MultiAxis<axis_cnt>::moveAll(double (&relative)[axis_cnt])
  {
    for (int i = 0; i < axis_cnt; i++)
      axis[i].move(relative[i]);
  }
#endif
