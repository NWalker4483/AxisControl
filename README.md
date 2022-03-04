# Differential Axis Control

Simple abstract classes for real-time single and multi axis speed/acceleration control.
## Setup 
sudo make install 


## Overview
### MultiAxis<num_axis>

```Meant for when a change in the speed of one motor can affect the speed of multiple axes. So the speed of each axis must be managed simultaneously```

|||
|---|---|
|<img src="imgs/Differential-drive.png" width= "400">|<img src="imgs/wrists.jpeg" width= "400">|

This class will instantiate a list of ```Axis``` objects automatically and then the corresponding override functions will allow the same level of control but with the consideration that changes on one Axis may affect other axes.

``` cpp

class MyRobot: MultiAxis<3>{

unsigned int getTime() override {};
/* If the time passed since isn't passed into the run function then this allows the 
object to grab how many units of time passed since the program started and compute speed + acceleration. 
            e.g millis();
*/

void computeAxisPositions(double *axis_positions) override {};
/* this function is passed a pointer in which to store the current position of each controlled axis in order */

void updateMotorSpeeds(double *axis_speeds) override {};
/* this function is passed a pointer to all of the current target speeds that each axis should attempt to achieve. The motor speeds should then be calculated by the user and applied for proper control*/

void pollMotors() override {};
/* this function is meant to stand in the place of anything that needs to be called as often as possible for the motors to move at an appropriate speed

    e.g AccelStepper::runSpeed()
*/
}
```

### Axis
``` Can be used for controlling a single axis at a time ```

<img src="imgs/collab.png" width= "400">

When creating a child class access you can override

``` cpp
/*
 These functions work basically the same as the ones I described above but for a single access it's not necessary to try and override these if you just want to use the MultiAxis class. 
*/
virtual unsigned int getTime();
virtual void computePosition(double *axis_position);
virtual void updateMotorSpeed(double *axis_speed);
virtual void pollMotor() {};
```
