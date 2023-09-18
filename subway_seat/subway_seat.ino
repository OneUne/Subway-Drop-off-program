#include <Wire.h>
#define LINE_FEED 10
#define CR 13

int input ;
int second;
int light;
int distance;

int GREEN = 6, AGREEN = 2, ARED = 3, MGREEN = 4, MRED = 5;
// GREEN, A(amber)GREEN, A(mber)RED는 옆자리 전구들
// M(ain)GREEN, M(ain)RED는 정기형님 자리꺼

int motor = 9;

int a = 1;
int b = 1; // 자리 처음 앉았을 때만 모터 돌아라
int c = 1;

void HandleEvent()  /* 이 함수가 데이터 통신을 처리 */ {
  distance = Wire.read() ;
}

void setup() {
  Serial.begin(9600) ;
  Wire.begin() ;
  Wire.onReceive( HandleEvent);

  pinMode(GREEN, OUTPUT);
  pinMode(AGREEN, OUTPUT);
  pinMode(ARED, OUTPUT);
  pinMode(MGREEN, OUTPUT);
  pinMode(MRED, OUTPUT);
  digitalWrite(GREEN, HIGH);
  digitalWrite(AGREEN, HIGH);
  digitalWrite(ARED, HIGH);
  digitalWrite(MGREEN, LOW);
  digitalWrite(MRED, HIGH);
  
  pinMode(motor, OUTPUT);
}

void loop() {
  while (c) {
    if (distance < 20){
      if( Serial.available() > 0 ) {
        Wire.beginTransmission( 255 ) ; /* 255번slave와통신시작*/
        char pc = Serial.peek();
        if (pc != LINE_FEED && pc != CR) {
          input = Serial.parseInt() ;
          Wire.write( input ) ; /* SDA를통해데이터를전달*/
          Wire.endTransmission() ;
        }
        else {
          Serial.read(); /* 불필요한 글자(CR이나 LF)를 큐에서 제거 */
        }
      }
    }
    break;
  }

  c = 0;
  
  while (a) {
    delay(5000); // 5초 후 앉아있던 사람 사라져요
    digitalWrite(MRED, LOW);
    a = 0; // 다시는 이 루프에 들어오지 않습니당
  }
  
  
  while (b) { // 이제 미스터 정기가 앉습니다
    light = analogRead(A1); // 라이트 센서 A1에 꽂아주세용 @예원
    if (light < 30) { // 그가 앉으면
      analogWrite(motor, 255); // 블루투스 보내라는 진동
    }
    if (Serial.available() > 0) { // 블루투스를 보내면
      analogWrite(motor, 0);
      b = 0; // 다시 이 루프에 들어오지 않습니다
      break;
    }
  }
  
  Serial.println(input);
  switch (input) {
    case 0:
    case -1:
    case -2:
    case -3:
    case -4:
      digitalWrite(MRED, LOW);
      digitalWrite(MGREEN, HIGH);
      break;
    case -5:
    case -6:
      digitalWrite(MRED, HIGH);
      digitalWrite(MGREEN, HIGH);
      break;
    case -7:
    case -8:
      digitalWrite(MRED, HIGH);
      digitalWrite(MGREEN, LOW);
    case -9:
      analogWrite(motor, 255);
      while (1) {
        if (light > 30) { // 그가 일어나면 전구와 모터가 꺼져요 !
          digitalWrite(MRED, LOW);
          digitalWrite(MGREEN, LOW);
          analogWrite(motor, 0);
          break;
        }
      }
  }

  
  delay(1000); // 1초에 second 하나씩 감소해요
  input--;
}
