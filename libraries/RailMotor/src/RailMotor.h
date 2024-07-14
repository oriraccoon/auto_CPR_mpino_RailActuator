#ifndef RailMotor_h
#define RailMotor_h

#include <Arduino.h>

class RailMotor {
  public:
    RailMotor(int pul, int dir, int speed = 10);

    void setup();
    void moveForward();
    void moveBackward();

  private:
    int _pul, _dir;
    int _speed;
};

#endif
