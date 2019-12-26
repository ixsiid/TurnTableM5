#ifndef STEPS_CONFIGURE_H
#define STEPS_CONFIGURE_H

#include "ipage.h"
#include "motor.h"

class StepsConfigure : public IPage {
  public:
    StepsConfigure(Motor * motor);

    virtual void start ();
    virtual void update();
    virtual void onPressedB();
    virtual void onPressedC();

  private:
    Motor * motor;
    void addSteps(int steps);
    void printStatus(MotorStatus * stats);
};

#endif
