#include "RailMotor.h"

RailMotor::RailMotor(int pul, int dir, int speed)
{
    _pul = pul;
    _dir = dir;
    _speed = speed;
}

void RailMotor::setup()
{
    pinMode(_pul, OUTPUT);
    pinMode(_dir, OUTPUT);
}

void RailMotor::moveForward()
{
    digitalWrite(_dir, HIGH);
    digitalWrite(_pul, HIGH);
    delayMicroseconds(_speed);
    digitalWrite(_pul, LOW);
    delayMicroseconds(_speed);
}

void RailMotor::moveBackward()
{
    digitalWrite(_dir, LOW);
    digitalWrite(_pul, HIGH);
    delayMicroseconds(_speed);
    digitalWrite(_pul, LOW);
    delayMicroseconds(_speed);
}
