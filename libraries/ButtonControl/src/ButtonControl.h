#ifndef BUTTON_CONTROL_H
#define BUTTON_CONTROL_H

#include <Arduino.h>

class ButtonControl {
public:
    ButtonControl(int sleepPin = -1, int stopPin = -1, int startPin = -1);

    void Button_initialize();
    void checkButtons();
    void startAction();
    void stopAction();
    void sleepAction();

    bool isStart();
    bool isStop();
    bool isSleep();

private:
    int _sleepPin;
    int _stopPin;
    int _startPin;

    volatile bool _startFlag = false;
    volatile bool _stopFlag; = false;
    volatile bool _sleepFlag = false;
};

#endif // BUTTON_CONTROL_H
