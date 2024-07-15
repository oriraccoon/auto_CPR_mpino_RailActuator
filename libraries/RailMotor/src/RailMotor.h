#ifndef RailMotor_h
#define RailMotor_h

#include <Arduino.h>

class RailMotor {
  public:
    RailMotor(int pul, int dir, int speed = 10);
    RailMotor(int pul, int dir, int speed = 10, int limit_for, int limit_back);
    void setup();
    void moveForward();
    void moveBackward();
    int attached_limit(int limit);
    void Reset();

  private:
    int _pul, _dir;
    int _speed;
    int _limit_for, _limit_back;
};

#endif
