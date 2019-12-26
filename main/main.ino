#include <M5Stack.h>
#include <Stepper.h>

#include "rotate.h"


#define MOTOR_1 (16)
#define MOTOR_2 (2)
#define MOTOR_3 (17)
#define MOTOR_4 (5)

#define MOTOR_STEPS (2048)
Stepper myStepper(MOTOR_STEPS, MOTOR_1, MOTOR_2, MOTOR_3, MOTOR_4);

class Motor {
  private:
    MotorStatus stats;

    void stopMotor() {
      digitalWrite(MOTOR_1, LOW);
      digitalWrite(MOTOR_2, LOW);
      digitalWrite(MOTOR_3, LOW);
      digitalWrite(MOTOR_4, LOW);
    }

  public:
    Motor() {
      myStepper.setSpeed(2);
      setRotateSteps(8);
      resetCount();
    }

    void setRotateSteps(int steps) {
      stats.steps = steps;
      stats.phase = steps * 360.0 / MOTOR_STEPS;
    }

    void resetCount() {
      stats.sum = 0.0f;
      stats.count = 0;
    }

    void step(bool clockwise) {
      int k = clockwise * 2 - 1;
      stats.count += k;
      stats.sum += k * stats.phase;
      myStepper.step(k * stats.steps);
    }

    MotorStatus * getStatus() {
      return &stats;
    }
};

class Page {
  public:
    char TextB[8] = "non";
    char TextC[8] = "non";

    virtual void start () {
      M5.Lcd.setTextSize(3);
      M5.Lcd.setCursor(5, 30);
      M5.Lcd.print("It is Empty page.");
    }

    virtual void update() {}
    virtual void onPressedB() {}
    virtual void onPressedC() {}
};


class Operation : public Page {
  public:
    Operation(Motor * motor) {
      this->motor = motor;

      strcpy(TextB, "Reverse");
      strcpy(TextC, "Forward");
    }

    virtual void start() {
      M5.Lcd.setTextSize(3);
      M5.Lcd.setCursor(5, 0);
      M5.Lcd.printf("Phase:");
      M5.Lcd.setCursor(5, 30);
      M5.Lcd.print("Count:");
      M5.Lcd.setCursor(5, 90);
      M5.Lcd.print("Sum Phase:");

      printStatus(motor->getStatus());
    }

    virtual void update() {}

    virtual void onPressedB() {
      motor->step(false);
      printStatus(motor->getStatus());
    }

    virtual void onPressedC() {
      motor->step(true);
      printStatus(motor->getStatus());
    }

  private:
    Motor * motor;

    void printStatus(MotorStatus * stats) {
      M5.Lcd.setTextSize(3);
      M5.Lcd.setCursor(120, 0);
      M5.Lcd.printf("%.2f'", stats->phase);
      M5.Lcd.setCursor(15, 63);
      M5.Lcd.printf("%5d", stats->count);
      M5.Lcd.setCursor(15, 123);
      M5.Lcd.printf("%8.2f", stats->sum);
    }
};

class StepsConfigure : public Page {
  public:
    StepsConfigure(Motor * motor) {
      this->motor = motor;

      strcpy(TextB, "-1");
      strcpy(TextC, "+1");
    }

    virtual void start() {
      M5.Lcd.setTextSize(3);
      M5.Lcd.setCursor(5, 0);
      M5.Lcd.printf("Rotate steps:");
      M5.Lcd.setCursor(5, 60);
      M5.Lcd.print("Phase:");

      printStatus(motor->getStatus());
    }

    virtual void onPressedB() {
      addSteps(-1);
    }

    virtual void onPressedC() {
      addSteps(+1);
    }

  private:
    Motor * motor;

    void addSteps(int steps) {
      int s = motor->getStatus()->steps + steps;
      if (s <= 0) s = 1;
      motor->setRotateSteps(s);
      printStatus(motor->getStatus());
    }

    void printStatus(MotorStatus * stats) {
      M5.Lcd.setTextSize(3);
      M5.Lcd.setCursor(15, 33);
      M5.Lcd.printf("%3d", stats->steps);
      M5.Lcd.setCursor(15, 93);
      M5.Lcd.printf("%.2f'", stats->phase);
    }
};

Motor motor;

int groupIndex = -1;
int pageIndex = -1;

#define GROUPNUM 2
Page ** groups[GROUPNUM];
short bgcolors[GROUPNUM] = { TFT_BLACK, TFT_NAVY };

int pageNum[GROUPNUM] = {2, 1};
Page * manual[2];
Page * autos[1];

Operation operation(&motor);
StepsConfigure steps(&motor);
Page autoOperation = Page();

void setup() {
  groups[0] = manual;
  groups[1] = autos;

  manual[0] = &operation;
  manual[1] = &steps;

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

  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(40, 220);
  M5.Lcd.print("Next");
  M5.Lcd.setCursor(145, 220);
  M5.Lcd.print(groups[groupIndex][pageIndex]->TextB);
  M5.Lcd.setCursor(240, 220);
  M5.Lcd.print(groups[groupIndex][pageIndex]->TextC);
}

bool groupChanging = false;

void loop() {
  groups[groupIndex][pageIndex]->update();

  if (!groupChanging) {
    if (M5.BtnA.pressedFor(1200)) {
      groupChanging = true;
      nextGroup();
    } else if (M5.BtnA.wasReleased()) {
      next();
    } else if (M5.BtnB.wasReleased()) {
      groups[groupIndex][pageIndex]->onPressedB();
    } else if (M5.BtnC.wasReleased()) {
      groups[groupIndex][pageIndex]->onPressedC();
    }
  } else {
    if (M5.BtnA.wasReleased()) {
      groupChanging = false;
    }
  }

  M5.update();
}
