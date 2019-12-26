#include <M5Stack.h>
#include <Stepper.h>
#include "motor.h"

Stepper myStepper(MOTOR_STEPS, MOTOR_1, MOTOR_2, MOTOR_3, MOTOR_4);

void Motor::stopMotor() {
  digitalWrite(MOTOR_1, LOW);
  digitalWrite(MOTOR_2, LOW);
  digitalWrite(MOTOR_3, LOW);
  digitalWrite(MOTOR_4, LOW);
}

Motor::Motor() {
  myStepper.setSpeed(2);
  setRotateSteps(8);
  resetCount();
}

void Motor::setRotateSteps(int steps) {
  stats.steps = steps;
  stats.phase = steps * 360.0 / MOTOR_STEPS;
}

void Motor::resetCount() {
  stats.sum = 0.0f;
  stats.count = 0;
}

void Motor::step(bool clockwise) {
  int k = clockwise * 2 - 1;
  stats.count += k;
  stats.sum += k * stats.phase;
  myStepper.step(k * stats.steps);
}

MotorStatus * Motor::getStatus() {
  return &stats;
}
