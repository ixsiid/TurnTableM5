#ifndef OPERATION_H
#define OPERATION_H

#include "ipage.h"
#include "motor.h"

class Operation : public IPage {
  public:
    Operation(Motor * motor);

    virtual void start ();
    virtual void update();
    virtual void onPressedB();
    virtual void onPressedC();

  private:
    Motor * motor;
    void printStatus(MotorStatus * stats);
};

#endif
