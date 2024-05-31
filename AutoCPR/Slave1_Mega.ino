#include <Wire.h>

int SensorPin = A0;
const int buttonPin = 2; // 버튼 핀 번호 설정
volatile bool buttonPressed = false; // 인터럽트에 의해 변경될 변수

void setup() {
  Wire.begin(8); // 슬레이브 주소 8로 I2C 버스 시작
  pinMode(buttonPin, INPUT_PULLUP); // 버튼 핀을 입력으로 설정, 내부 풀업 저항 사용
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
    int sensorValue = analogRead(SensorPin); // 센서에서 데이터 읽기
    Wire.write(0); // 버튼이 눌리지 않았다는 신호 전송
    Wire.write(sensorValue >> 8); // 센서 데이터의 상위 8비트 전송
    Wire.write(sensorValue & 0xFF); // 센서 데이터의 하위 8비트 전송
  }
}