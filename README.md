# Axis Control

Simple abstract classes for real-time single and multi axis speed/acceleration control.

## Axis
``` Meant for controlling a single axis at a time```

<img src="imgs/collab.png" width= "400">

When creating a child class access you can override

``` cpp
virtual unsigned int getTime();
virtual void computePosition();
virtual void updateMotorSpeed(double axis_speed);
virtual void pollMotor() {};
```
## MultiAxis<num_axis>

```Meant for when a change in the speed of one motor can affect the speed of multiple axes. So the speed of each axis must be managed simultaneously```

|||
|---|---|
|<img src="imgs/Differential-drive.png" width= "400">|<img src="imgs/wrists.jpeg" width= "400">|

This class will instantiate a list of ```Axis``` objects automatically and then the corresponding override functions will allow the same level of control but with the consideration that changes on one Axis may affect other axes.

``` cpp
virtual unsigned int getTime();
virtual void computeAxisPositions(double *axis_positions);
virtual void updateMotorSpeeds(double *axis_speeds);
virtual void pollMotors() {};
```