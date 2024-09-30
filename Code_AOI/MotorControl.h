#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H

#include <AccelStepper.h>

class MotorControl {
  public:
    MotorControl(int stepPin, int dirPin);
    void begin();
    void setDirection(bool direction);
    void moveSteps(int steps, int speed);
    void stop();
    void update();
    bool isRunning();
    bool getDirection();
    void emergencyStop();


  private:
    AccelStepper stepper;
    bool motorRunning;
    bool direction;
};

#endif
