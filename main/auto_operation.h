#ifndef AUTO_OPERATION_H
#define AUTO_OPERATION_H

#include "ipage.h"
#include "motor.h"

class AutoOperation : public IPage {
  public:
    AutoOperation(Motor * motor);

    virtual void start();
    virtual void update();
    virtual void onPressedB();
    virtual void onPressedC();

  private:
    Motor * motor;

    void printStatus(MotorStatus * stats);
};


#endif AUTO_OPERATION_H
