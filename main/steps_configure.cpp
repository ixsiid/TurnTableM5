#include <M5Stack.h>
#include "steps_configure.h"

StepsConfigure::StepsConfigure(Motor * motor) {
  this->motor = motor;

  strcpy(TextB, "-1");
  strcpy(TextC, "+1");
}

void StepsConfigure::start() {
  M5.Lcd.setTextSize(3);
  M5.Lcd.setCursor(5, 0);
  M5.Lcd.printf("Rotate steps:");
  M5.Lcd.setCursor(5, 60);
  M5.Lcd.print("Phase:");

  printStatus(motor->getStatus());
}

void StepsConfigure::update() {}

void StepsConfigure::onPressedB() {
  addSteps(-1);
}

void StepsConfigure::onPressedC() {
  addSteps(+1);
}


void StepsConfigure::addSteps(int steps) {
  int s = motor->getStatus()->steps + steps;
  if (s <= 0) s = 1;
  motor->setRotateSteps(s);
  printStatus(motor->getStatus());
}

void StepsConfigure::printStatus(MotorStatus * stats) {
  M5.Lcd.setTextSize(3);
  M5.Lcd.setCursor(15, 33);
  M5.Lcd.printf("%3d", stats->steps);
  M5.Lcd.setCursor(15, 93);
  M5.Lcd.printf("%.2f'", stats->phase);
}
