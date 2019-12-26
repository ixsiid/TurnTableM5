#ifndef ROTATE_H
#define ROTATE_H

typedef struct _MotorStatus {
  float phase;
  float sum;
  int count;
  int steps;
} MotorStatus;

#endif
