#include <Wire.h>

#define btn 2
#define Actuator_FOR 6
#define Actuator_BACK 7
#define Actuator_PWM 5

volatile bool stopp = false;
volatile bool do_actuator = false;
volatile bool dooo = false;

void setup() {
  Wire.begin(10); // 슬레이브 주소 0x04로 I2C 통신 시작
  pinMode( btn, INPUT_PULLUP );
  pinMode( Actuator_PWM, OUTPUT       );
  pinMode( Actuator_FOR, OUTPUT       );
  pinMode( Actuator_BACK, OUTPUT       );
  attachInterrupt(digitalPinToInterrupt(btn), R_btn, FALLING);
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent); // 데이터 수신 이벤트 핸들러 등록
  Serial.begin(115200);
}

void loop() {
  while(do_actuator){
        if(!do_actuator) break;
        digitalWrite( Actuator_FOR, HIGH );
        digitalWrite( Actuator_BACK, LOW );
        analogWrite( Actuator_PWM, 255 );
        delay( 300 );
        if(!do_actuator) break;
        digitalWrite( Actuator_FOR, LOW );
        digitalWrite( Actuator_BACK, HIGH );
        analogWrite( Actuator_PWM, 255 );
        delay( 300 );
    }
}
void R_btn(){
  dooo = true;
  delay(100);
}
volatile int count = 0;
void requestEvent(){
  if (!digitalRead(btn)) Wire.write(0);
  else if (dooo){
    Wire.write(1);
    dooo = false;
  }
  
}

void receiveEvent(int howMany) {
  while (Wire.available()) { // 수신된 데이터 개수만큼 반복
    int data = Wire.read(); // 데이터 읽기
    switch(data){
      case 1 : do_actuator = true;
              break;
      case 2 : do_actuator = false;
              break;
      default : break;
    }
    
  }
}