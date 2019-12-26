#include <M5Stack.h>

#include "turn_table.h"
#include "motor.h"
#include "ipage.h"
#include "operation.h"
#include "steps_configure.h"
#include "auto_operation.h"

Motor motor;

int groupIndex = -1;
int pageIndex = -1;

#define GROUPNUM 2
IPage ** groups[GROUPNUM];
short bgcolors[GROUPNUM] = { TFT_BLACK, TFT_NAVY };

int pageNum[GROUPNUM] = {2, 1};
IPage * manual[2];
IPage * autos[1];

Operation operation(&motor);
StepsConfigure steps(&motor);
AutoOperation autoOperation(&motor);

void setup() {
  groups[0] = manual;
  groups[1] = autos;

  // group 1
  manual[0] = &operation;
  manual[1] = &steps;

  // group 2
  autos[0] = &autoOperation;

  M5.begin();
  nextGroup();
}

void nextGroup() {
  if (++groupIndex >= GROUPNUM) groupIndex -= GROUPNUM;
  pageIndex = -1;
  next();
}

void next() {
  if (++pageIndex >= pageNum[groupIndex]) pageIndex -= pageNum[groupIndex];

  M5.Lcd.clear(bgcolors[groupIndex]);
  groups[groupIndex][pageIndex]->start();

  printMenu();
}

void printMenu() {
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(40, 220);
  M5.Lcd.print("Next");
  M5.Lcd.setCursor(145, 220);
  M5.Lcd.print(groups[groupIndex][pageIndex]->TextB);
  M5.Lcd.setCursor(240, 220);
  M5.Lcd.print(groups[groupIndex][pageIndex]->TextC);
}

bool btnALongPressing = false;

void loop() {
  groups[groupIndex][pageIndex]->update();

  if (btnALongPressing) {
    if (M5.BtnA.wasReleased()) btnALongPressing = false;
  } else {
    if (M5.BtnA.pressedFor(1200)) {
      btnALongPressing = true;
      nextGroup();
    } else if (M5.BtnA.wasReleased()) {
      next();
    } else if (M5.BtnB.wasReleased()) {
      groups[groupIndex][pageIndex]->onPressedB();
    } else if (M5.BtnC.wasReleased()) {
      groups[groupIndex][pageIndex]->onPressedC();
    }
  }

  M5.update();
}
