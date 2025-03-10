#include <Wire.h>
#include <PulseSensorPlayground.h>
#include <SoftwareSerial.h>

#define PULSE_sensor 0
#define btn 2
#define BT_RXD 8
#define BT_TXD 9
#define Actuator_FOR 6
#define Actuator_BACK 7
#define Actuator_PWM 5

SoftwareSerial bluetooth(BT_RXD, BT_TXD);

volatile bool stopp = false;
volatile bool do_actuator = false;
volatile bool dooo = false;
volatile bool LOWBPM = false;
volatile bool NORMALBPM = false;

unsigned long lastBeatTime = 0;
unsigned long BeatInterval = 0;
const long minBeatInterval = 700;
unsigned long currentTime = 0;
volatile int Threshold = 550;

const int minBPM = 50;
const int maxBPM = 180;
const int maxBPMChange = 20; // 허용되는 최대 심박수 변화 폭
const int EmerBPM = 66;
unsigned int myBPM = 0;
unsigned int lastValidBPM = 80; // 마지막 유효한 심박수
int push_count = 0;

PulseSensorPlayground pulseSensor;

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
  bluetooth.begin(9600);
  
  pulseSensor.analogInput(PULSE_sensor);
  pulseSensor.setThreshold(Threshold);
  if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");  //This prints one time at Arduino power-up, or on Arduino reset.  
  }
}

	
int checkBeatValidity( int bpm, unsigned long interval )
{
	if ( bpm >= minBPM && bpm <= maxBPM && interval >= minBeatInterval )
	{
		return 9; // 유효한 심박수
	}
	else if ( interval < minBeatInterval )
	{
		return 7; // 심박 간격이 너무 짧음
	}
	else if ( bpm < minBPM )
	{
		return 8; // 심정지 상태
	}
	else
	{
		return 6; // 유효하지 않은 심박수
	}
}
void _PULSE() {
  if (pulseSensor.sawStartOfBeat()) {
    currentTime = millis();
    BeatInterval = currentTime - lastBeatTime;
    int newBPM = pulseSensor.getBeatsPerMinute();
/*
    Serial.print("New BPM: ");
    Serial.println(newBPM);
    Serial.print("Last Valid BPM: ");
    Serial.println(lastValidBPM);
    Serial.print("Beat Interval: ");
    Serial.println(BeatInterval);
*/
    int bpmChange = newBPM - lastValidBPM;
    if (bpmChange < 0) bpmChange = -bpmChange;

    if (bpmChange > maxBPMChange) {
      Serial.println("Invalid BPM change detected.");
      return;
    }

    myBPM = newBPM;

    switch (checkBeatValidity(myBPM, BeatInterval)) {
      case 9:
        Serial.println(myBPM);
        NORMALBPM = true;
        LOWBPM = false;
        lastBeatTime = currentTime;
        lastValidBPM = myBPM;
        break;
      case 8:
        Serial.println("Warning: BPM too low.");
        NORMALBPM = false;
        LOWBPM = true;
        break;
      case 7:
        Serial.println("Warning: BPM interval too short.");
        lastBeatTime = currentTime;
        break;
      case 6:
        Serial.println("Invalid BPM detected.");
        lastBeatTime = currentTime;
        break;
      default:
        break;
    }
    
    bluetooth.print(myBPM); // 블루투스를 통해 신호 값을 전송
    bluetooth.print(","); // 데이터를 구분하기 위해 쉼표 추가
    bluetooth.println(currentTime / 1000); // 블루투스를 통해 경과 시간을 전송
    bluetooth.print(",");
    bluetooth.print(push_count);
  }

}

void loop() {
  if (bluetooth.available()) {
    Serial.write(bluetooth.read());
  }
  if (Serial.available()) {
    bluetooth.write(Serial.read());
  }

  while (do_actuator) {
    if (!do_actuator) break;
    digitalWrite(Actuator_FOR, HIGH);
    digitalWrite(Actuator_BACK, LOW);
    analogWrite(Actuator_PWM, 255);
    delay(300);
    digitalWrite(Actuator_FOR, LOW);
    digitalWrite(Actuator_BACK, HIGH);
    analogWrite(Actuator_PWM, 255);
    delay(300);
    _PULSE();
  }
  _PULSE();
  delay(500);
}

void R_btn(){
  dooo = true;
  delay(100);
}

void requestEvent(){
  if(LOWBPM && !NORMALBPM) Wire.write(2);
  else if(!LOWBPM && NORMALBPM) Wire.write(9);  
  else if (!digitalRead(btn)) Wire.write(0);
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