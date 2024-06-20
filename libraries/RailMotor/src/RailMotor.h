#ifndef RailMotor_h
#define RailMotor_h

#include <Arduino.h>

class RailMotor {
  public:
    RailMotor(int x_pul, int x_dir, int z_pul, int z_dir,
              int x_limit_b = -1, int x_limit_f = -1, int z_limit_b = -1, int z_limit_f = -1);

    void setup();
    void moveXForward();
    void moveXBackward();
    void moveZForward();
    void moveZBackward();
    void resetPosition();
    void setSpeed(int speed);

    bool isXLimitReached();
    bool isZLimitReached();

  private:
    int _x_pul, _x_dir, _z_pul, _z_dir;
    int _x_limit_b, _x_limit_f, _z_limit_b, _z_limit_f;
    int _speed; // 마이크로초 단위 딜레이
};

#endif
