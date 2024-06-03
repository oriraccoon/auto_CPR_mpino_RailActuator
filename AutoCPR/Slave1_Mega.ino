#include <Wire.h>

const int buttonPin = 2; // 버튼 핀 번호 설정
const int sensor1Pin = 3; // 센서 1 핀 번호 설정
const int sensor2Pin = 4; // 센서 2 핀 번호 설정
const int sensor3Pin = 5; // 센서 3 핀 번호 설정
const int sensor4Pin = 6; // 센서 4 핀 번호 설정

volatile bool buttonPressed = false; // 인터럽트에 의해 변경될 변수

void setup() {
  Wire.begin(8); // 슬레이브 주소 8로 I2C 버스 시작
  pinMode(buttonPin, INPUT_PULLUP); // 버튼 핀을 입력으로 설정, 내부 풀업 저항 사용
  pinMode(sensor1Pin, INPUT); // 센서 1 핀을 입력으로 설정
  pinMode(sensor2Pin, INPUT); // 센서 2 핀을 입력으로 설정
  pinMode(sensor3Pin, INPUT); // 센서 3 핀을 입력으로 설정
  pinMode(sensor4Pin, INPUT); // 센서 4 핀을 입력으로 설정
  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonInterrupt, FALLING); // 버튼 핀에 인터럽트 연결
  Wire.onRequest(requestEvent); // 데이터 요청 시 실행할 함수 등록
}

void loop() {
  // 메인 루프는 비워둠, 모든 로직은 인터럽트와 onRequest에서 처리
}

void buttonInterrupt() {
  buttonPressed = true; // 버튼이 눌림
}

void requestEvent() {
  if (buttonPressed) {
    Wire.write(1); // 버튼이 눌렸다는 신호 전송
    buttonPressed = false; // 상태 초기화
  } else {
    int sensor1Value = digitalRead(sensor1Pin);
    int sensor2Value = digitalRead(sensor2Pin);
    int sensor3Value = digitalRead(sensor3Pin);
    int sensor4Value = digitalRead(sensor4Pin);

    if (sensor1Value == LOW) {
      Wire.write(2); // 센서 1 인식 신호 전송
    } else if (sensor2Value == LOW) {
      Wire.write(3); // 센서 2 인식 신호 전송
    } else if (sensor3Value == LOW) {
      Wire.write(4); // 센서 3 인식 신호 전송
    } else if (sensor4Value == LOW) {
      Wire.write(5); // 센서 4 인식 신호 전송
    } else {
      Wire.write(0); // 변화 없음을 나타내는 신호 전송
    }
  }
}