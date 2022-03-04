# Differential Axis Control

Simple abstract classes for single and multi axis speed/acceleration control.

## Setup 
mkdir build
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

    PID pids[3];
    joint_sensor[3];

unsigned int getTime() override {};
/* If the time passed since isn't passed into the run function then this allows the 
object to grab how many units of time passed since the program started and compute speed + acceleration. 
            e.g millis();
*/

void computeAxisPositions(double *axis_positions) override {
     for(int i = 0; i < 3; i++) axis_positions[2] = joint_sensor[i].value;
};
/* this function is passed a pointer in which to store the current position of each controlled axis in order */

void updateMotorSpeeds(double *axis_speeds) override { 
    pid[i].setpoint = axis_speeds[i] * reductions;
};
/* this function is passed a pointer to all of the current target speeds that each axis should attempt to achieve. 
The instantaneous motor speeds should then be calculated by the user and applied for proper control*/

void pollMotors() override {
   for(int i = 0; i < 3; i++) pid[i].update(axis[i].real_speed);
};
/* this function is meant to stand in the place of anything that needs to be called as often as 
possible for the motors to move at an appropriate speed

    e.g AccelStepper::runSpeed()
*/
}

MyRobot.axis[0].setTargetSpeed(5)// deg/s
MyRobot.axis[1].setTargetSpeed(10)// deg/s
MyRobot.axis[2].setTargetSpeed(4)// deg/s

MyRobot.setLimitMode(1); // Limit Acceleration
double home_pose[3] = {34, 0, 90};
MyRobot.moveAllTo(home_pose);

MyRobot.runToPositions();

```


### Axis
``` Can be used for controlling a single axis at a time ```

<img src="imgs/collab.png" width= "400">

``` cpp
/*
 These functions work basically the same as the ones I described above but for a single access it's not necessary to try and override these if you just want to use the MultiAxis class. 
*/
virtual unsigned int getTime();
virtual void computePosition(double *axis_position);
virtual void updateMotorSpeed(double *axis_speed);
virtual void pollMotor() {};
```
