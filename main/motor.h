#ifndef MOTOR_H
#define MOTOR_H

typedef struct _MotorStatus {
  float phase;
  float sum;
  int count;
  int steps;
} MotorStatus;

#define MOTOR_1 (16)
#define MOTOR_2 (2)
#define MOTOR_3 (17)
#define MOTOR_4 (5)

#define MOTOR_STEPS (2048)

class Motor {
  private:
    MotorStatus stats;
    void stopMotor();

  public:
    Motor();
    void setRotateSteps(int steps);
    void resetCount();
    void step(bool clockwise);
    MotorStatus * getStatus();
};

#endif
