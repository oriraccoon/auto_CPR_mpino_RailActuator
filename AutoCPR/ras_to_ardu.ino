void setup() {
  Serial.begin(9600); // 시리얼 통신 시작
}

void loop() {
  if (Serial.available() > 0) {
    int receivedValue = Serial.read(); // 데이터를 읽음

    if (receivedValue == '1') { // '1'을 받았을 때
      Serial.print("ok");
    }
  }
}