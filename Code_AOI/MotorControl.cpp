#include "MotorControl.h"

MotorControl::MotorControl(int stepPin, int dirPin)
  : stepper(AccelStepper::DRIVER, stepPin, dirPin), motorRunning(false), direction(false) {}

void MotorControl::begin() {
  stepper.setMaxSpeed(300);
  stepper.setAcceleration(70);
}

void MotorControl::setDirection(bool dir) {
  direction = dir;
  stepper.setPinsInverted(!dir, dir);
}

void MotorControl::moveSteps(int steps, int speed) {
  motorRunning = true;
  stepper.setSpeed(speed);
  stepper.moveTo(stepper.currentPosition() + steps);
}

void MotorControl::stop() {
  motorRunning = false;
  stepper.stop();
}

void MotorControl::update() {
  if (motorRunning && stepper.distanceToGo() != 0) {
    stepper.run();
  } else {
    motorRunning = false;
  }
}

void MotorControl::emergencyStop() {
  // Arresta immediatamente il motore
  stepper.stop(); // Arresta immediatamente il motore
  
  motorRunning = false; // Imposta il flag del motore in esecuzione su false
}

bool MotorControl::isRunning() {
  return motorRunning;
}

bool MotorControl::getDirection() {
  return direction;
}
