#ifndef BUTTON_CONTROL_H
#define BUTTON_CONTROL_H

#include <Arduino.h>

class ButtonControl {
public:
    ButtonControl(int sleepPin, int stopPin, int startPin = -1);

    void initialize();
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

    volatile bool _startFlag;
    volatile bool _stopFlag;
    volatile bool _sleepFlag;

    void handleSleepButton();
    void handleStopButton();
    void handleStartButton();
};

#endif // BUTTON_CONTROL_H
