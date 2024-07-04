#include <Wire.h>
#include <PulseSensorPlayground.h>

#define PULSE_sensor 0
#define btn 2
#define Actuator_FOR 6
#define Actuator_BACK 7
#define Actuator_PWM 5

volatile bool stopp = false;
volatile bool do_actuator = false;
volatile bool dooo = false;
volatile bool LOWBPM = false;
volatile bool NORMALBPM = false;

unsigned long long lastBeatTime = 0;
unsigned long long BeatInterval = 0;
const long long minBeatInterval = 700;
unsigned long long currentTime = 0;
volatile int Threshold = 550;

const int minBPM = 68;
const int maxBPM = 180;
int myBPM = 0;


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

void loop() {
  while(do_actuator){
        if(!do_actuator) break;
        digitalWrite( Actuator_FOR, HIGH );
        digitalWrite( Actuator_BACK, LOW );
        analogWrite( Actuator_PWM, 255 );
        delay( 300 );
        digitalWrite( Actuator_FOR, LOW );
        digitalWrite( Actuator_BACK, HIGH );
        analogWrite( Actuator_PWM, 255 );
        delay( 300 );
        if ( pulseSensor.sawStartOfBeat() )
        {
          currentTime = millis();
          BeatInterval = currentTime - lastBeatTime;
          myBPM = pulseSensor.getBeatsPerMinute();
          switch ( checkBeatValidity( myBPM, BeatInterval ) )
          {
            case 9:
              Serial.println( myBPM );
              NORMALBPM = true;
              LOWBPM = false;
              lastBeatTime = currentTime;
              break;
            case 8 :
              Serial.println("warning");
              NORMALBPM = false;
              LOWBPM = true;
              break;
            case 7 :
              Serial.println("so fast BPM");
              lastBeatTime = currentTime;
              break;
            case 6 :
              Serial.println("invaild BPM");
              lastBeatTime = currentTime;
              break;
            default :
              break;
          }
        }
  }
  if ( pulseSensor.sawStartOfBeat() )
	{
		currentTime = millis();
		BeatInterval = currentTime - lastBeatTime;
		myBPM = pulseSensor.getBeatsPerMinute();
    switch ( checkBeatValidity( myBPM, BeatInterval ) )
		{
			case 9:
				Serial.println( myBPM );
        NORMALBPM = true;
        LOWBPM = false;
				lastBeatTime = currentTime;
				break;
			case 8 :
        Serial.println("warning");
        NORMALBPM = false;
        LOWBPM = true;
				break;
			case 7 :
        Serial.println("so fast BPM");
				lastBeatTime = currentTime;
				break;
			case 6 :
        Serial.println("invaild BPM");
				lastBeatTime = currentTime;
				break;
			default :
				break;
		}
    
  }
}
void R_btn(){
  dooo = true;
  delay(100);
}

volatile int count = 0;
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