#include <RailMotor.h>
#include <Arduino.h>

RailMotor::RailMotor(int pul, int dir, int speed)
{
    _pul = pul;
    _dir = dir;
    _speed = speed;
}

RailMotor::RailMotor(int pul, int dir, int limit_for, int limit_back, int speed)
{
    _pul = pul;
    _dir = dir;
    _limit_for = limit_for;
    _limit_back = limit_back;
    _speed = speed;
}

void RailMotor::setup()
{
    pinMode(_pul, OUTPUT);
    pinMode(_dir, OUTPUT);
}

void RailMotor::moveForward()
{
    while ( !attached_limit(_limit_for))
    {
        digitalWrite(_dir, HIGH);
        digitalWrite(_pul, HIGH);
        delayMicroseconds(_speed);
        digitalWrite(_pul, LOW);
        delayMicroseconds(_speed);
    }
}

void RailMotor::moveBackward()
{
    while ( !attached_limit(_limit_back))
    {
        digitalWrite(_dir, LOW);
        digitalWrite(_pul, HIGH);
        delayMicroseconds(_speed);
        digitalWrite(_pul, LOW);
        delayMicroseconds(_speed);
    }
}

int RailMotor::attached_limit(int limit)
{
    int flag;
    flag = digitalRead(limit);
    return flag;
}

void RailMotor::Reset(){
    moveBackward();
}