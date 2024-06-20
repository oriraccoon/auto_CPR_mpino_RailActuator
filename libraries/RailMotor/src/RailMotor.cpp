#include "RailMotor.h"

RailMotor::RailMotor(int x_pul, int x_dir, int z_pul, int z_dir,
                     int x_limit_b, int x_limit_f, int z_limit_b, int z_limit_f)
{
    _x_pul = x_pul;
    _x_dir = x_dir;
    _z_pul = z_pul;
    _z_dir = z_dir;
    _x_limit_b = x_limit_b;
    _x_limit_f = x_limit_f;
    _z_limit_b = z_limit_b;
    _z_limit_f = z_limit_f;
    _speed = 1000; // 디폴트 속도 설정 (예: 1000 마이크로초)
}

void RailMotor::setup()
{
    pinMode(_x_pul, OUTPUT);
    pinMode(_x_dir, OUTPUT);
    pinMode(_z_pul, OUTPUT);
    pinMode(_z_dir, OUTPUT);

    if (_x_limit_b != -1) pinMode(_x_limit_b, INPUT_PULLUP);
    if (_x_limit_f != -1) pinMode(_x_limit_f, INPUT_PULLUP);
    if (_z_limit_b != -1) pinMode(_z_limit_b, INPUT_PULLUP);
    if (_z_limit_f != -1) pinMode(_z_limit_f, INPUT_PULLUP);
}

void RailMotor::moveXForward()
{
    digitalWrite(_x_dir, HIGH);
    digitalWrite(_x_pul, HIGH);
    delayMicroseconds(_speed);
    digitalWrite(_x_pul, LOW);
    delayMicroseconds(_speed);
}

void RailMotor::moveXBackward()
{
    digitalWrite(_x_dir, LOW);
    digitalWrite(_x_pul, HIGH);
    delayMicroseconds(_speed);
    digitalWrite(_x_pul, LOW);
    delayMicroseconds(_speed);
}

void RailMotor::moveZForward()
{
    digitalWrite(_z_dir, HIGH);
    digitalWrite(_z_pul, HIGH);
    delayMicroseconds(_speed);
    digitalWrite(_z_pul, LOW);
    delayMicroseconds(_speed);
}

void RailMotor::moveZBackward()
{
    digitalWrite(_z_dir, LOW);
    digitalWrite(_z_pul, HIGH);
    delayMicroseconds(_speed);
    digitalWrite(_z_pul, LOW);
    delayMicroseconds(_speed);
}

void RailMotor::resetPosition()
{
    while (!isXLimitReached() || !isZLimitReached())
    {
        if (_x_limit_b != -1 && !digitalRead(_x_limit_b))
        {
            moveXBackward();
        }
        if (_z_limit_b != -1 && !digitalRead(_z_limit_b))
        {
            moveZBackward();
        }

        if (_x_limit_b == -1 && _z_limit_b == -1)
        {
            // handle without limit switches
        }

        delayMicroseconds(_speed);

        // handle other conditions like sleep or stop buttons if needed
    }
}

void RailMotor::setSpeed(int speed)
{
    _speed = speed;
}

bool RailMotor::isXLimitReached()
{
    return (_x_limit_f != -1) ? digitalRead(_x_limit_f) : false;
}

bool RailMotor::isZLimitReached()
{
    return (_z_limit_f != -1) ? digitalRead(_z_limit_f) : false;
}
