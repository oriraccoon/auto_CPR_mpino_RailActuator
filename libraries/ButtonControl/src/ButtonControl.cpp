#include "ButtonControl.h"

ButtonControl::ButtonControl(int sleepPin, int stopPin, int startPin){
    _sleepPin = sleepPin;
    _stopPin = stopPin;
    _startPin = startPin;
}

void ButtonControl::Button_initialize() {
    pinMode(_sleepPin, INPUT_PULLUP);
    pinMode(_stopPin, INPUT_PULLUP);
    if (_startPin != -1) {
        pinMode(_startPin, INPUT_PULLUP);
    }
}

void ButtonControl::checkButtons() {
    if (digitalRead(_sleepPin)) Detect_SleepButton();
    if (digitalRead(_stopPin)) Detect_StopButton();
    if (_startPin != -1) {
        if (digitalRead(_startPin)) Detect_StartButton();
    }
}

void ButtonControl::startAction() {
    _startFlag = true;
    _stopFlag = false;
    _sleepFlag = false;
}

void ButtonControl::stopAction() {
    _stopFlag = true;
    _startFlag = false;
    _sleepFlag = false;
}

void ButtonControl::sleepAction() {
    _sleepFlag = !_sleepFlag;
    delay(300);  // 디바운스 처리
}

bool ButtonControl::isStart() {
    return _startFlag;
}

bool ButtonControl::isStop() {
    return _stopFlag;
}

bool ButtonControl::isSleep() {
    return _sleepFlag;
}