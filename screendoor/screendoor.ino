#include <SharpIR.h>
#include <Servo.h>
#include <Wire.h>

SharpIR sensor( SharpIR::GP2Y0A21YK0F, A0 );
Servo myservo; 
Servo myservo2;

int LED[7] = {2,3,4,5,6,7,8};
int seatleft = 0 ;    /* 데이터 통신으로 받은 값을 저장 */
int SVpin = 9; 

int BuzzerPin = 11;
char noteNames[] = {'C','D','E','F','G','a','b','c'};
int frequency[] = {1047,1175,1319,1397,1568,1760,1976,2093}; 
byte noteCount = sizeof(noteNames);

char score[] = "CC CC CC  DE EE EE  FG GG GG  Gabc G  Gabc C";
byte scoreLen = sizeof(score);
 
void HandleEvent()  /* 이 함수가 데이터 통신을 처리 */ {
  seatleft = Wire.read() ;
}

void Clear() {
  for ( int i = 0 ; i < 7; i++ ) digitalWrite(LED[i], HIGH) ;
}

void setup() {
  for ( int i = 0 ; i < 7; i++ )pinMode(LED[i],OUTPUT);
  Serial.begin( 9600 ); //Enable the serial comunication 
  Wire.begin(255) ;     /* Slave 자신의 주소를 255번으로 설정 */ 
  Wire.onReceive( HandleEvent ) ;  /* 데이터 수신시 호출 할 함수 설정*/
  myservo.attach(9);
  myservo2.attach(10); 
  //myservo.write(0); // 없어도 될 듯
}

void SetSegment(int i) {
  if( i != 1 && i != 4) digitalWrite( LED[0], LOW ) ; // A 
  if( i != 5 && i != 6) digitalWrite( LED[1], LOW ) ; // B 
  if( i != 2 ) digitalWrite( LED[2], LOW ) ;          // C 
  if( i != 1 && i != 4 && i != 7 ) digitalWrite( LED[3], LOW ) ; // D 
  if( i == 0 | i == 2 || i == 6 || i == 8  ) digitalWrite( LED[4], LOW ) ; // E 
  if( i == 0 || (i >= 4 && i <= 9)  ) digitalWrite( LED[5], LOW ) ; // F 
  if( i != 0 && i != 1 && i != 7  ) digitalWrite( LED[6], LOW ) ; // G
}

void playNote(char note, int duration){
  
  for (int i=0 ; i<noteCount ; i++)
  { if (noteNames[i] == note)
    tone(BuzzerPin, frequency[i], duration);
  }
    delay(duration);
}

void loop() { 
  Clear();
  int distance = sensor.getDistance(); //Calculate the distance in centimeters 
  Serial.println( "열차 진입 전"); //Print the value to the serial monitor 
  Serial.println( distance );
  
  if (distance < 15 ) { // 열차가 역에 진입할 때
    Serial.println( "열차 진입 후"); //Print the value to the serial monitor 
    Serial.println( distance );
    SetSegment(seatleft);
    
    for (int i=0 ; i<scoreLen ; i++)
    {int duration = 150;
    playNote(score[i], duration);
    }
    

    /*
    Wire.beginTransmission(  ) ; // master와 통신시작
    if( Serial.available() > 0 ) {
      Wire.write( distance ) ; // SDA를통해데이터를전달
      Wire.endTransmission() ;
    }
    */
    
    for(int j=0; j<90; j++){ // 서보모터를 통한 스크린 도어 열림 표시
      myservo.write(90-j);
      myservo2.write(j);
      delay(20);
    }
    delay(2000);
    for(int j=0; j<90; j++){
      myservo.write(j);
      myservo2.write(90-j);
      delay(20);
    }
      
    /*
    if (distance < 5){ // 열차가 거의 도착하면
      Serial.println("아예가까워진"); //Print the value to the serial monitor 
      Serial.println( distance );
      for(int j=0; j<90; j++){ // 서보모터를 통한 스크린 도어 열림 표시
        myservo.write(j);
        myservo2.write(j);
        delay(20);
      }
      delay(2000);
      for(int j=0; j<90; j++){
        myservo.write(90-j);
        myservo2.write(90-j);
        delay(20);
      }
    }
    */
    delay(5000);
  }
}
