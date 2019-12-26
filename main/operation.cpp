#include <M5Stack.h>
#include "operation.h"

Operation::Operation(Motor * motor) {
  this->motor = motor;

  strcpy(TextB, "Reverse");
  strcpy(TextC, "Forward");
}

void Operation::start() {
  M5.Lcd.setTextSize(3);
  M5.Lcd.setCursor(5, 0);
  M5.Lcd.printf("Phase:");
  M5.Lcd.setCursor(5, 30);
  M5.Lcd.print("Count:");
  M5.Lcd.setCursor(5, 90);
  M5.Lcd.print("Sum Phase:");

  printStatus(motor->getStatus());
}

void Operation::update() {}

void Operation::onPressedB() {
  motor->step(false);
  printStatus(motor->getStatus());
}

void Operation::onPressedC() {
  motor->step(true);
  printStatus(motor->getStatus());
}

void Operation::printStatus(MotorStatus * stats) {
  M5.Lcd.setTextSize(3);
  M5.Lcd.setCursor(120, 0);
  M5.Lcd.printf("%.2f'", stats->phase);
  M5.Lcd.setCursor(15, 63);
  M5.Lcd.printf("%5d", stats->count);
  M5.Lcd.setCursor(15, 123);
  M5.Lcd.printf("%8.2f", stats->sum);
}
