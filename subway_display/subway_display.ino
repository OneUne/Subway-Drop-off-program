#include <Wire.h>
#include <LiquidCrystal.h>
#include <Servo.h>

 int BuzzerPin = 9;

 char noteNames[] = {'C','D','E','F','G','a','b','c','d','e'};

 int frequency[] = {1047,1175,1319,1397,1568,1760,1976,2093,2349,2637}; 

 byte noteCount = sizeof(noteNames);

 int ACTION = 0;

 

 char score[] = "G c cGd e dca c dcG c";

 byte scoreLen = sizeof(score);


const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


void Clear() { /* LED를모두끔*/

}

void HandleEvent() {
  ACTION = Wire.read();
}

void setup() {
  Serial.begin( 9600 );
  Wire.begin(9);
  Wire.onReceive(HandleEvent);
  lcd.begin(16,2);
  pinMode(BuzzerPin, OUTPUT);
 
}

void loop() {
  if(ACTION == 1){
    lcd.setCursor(0,0);
    lcd.clear();
    lcd.print("THIS STOP IS"); 
    lcd.setCursor(0,1);
    lcd.print("SINSEOLDONG");
    delay(5000); //5초
 
    lcd.clear();
    lcd.print("THIS STOP IS"); 
    lcd.setCursor(0,1);
    lcd.print("JAEGIDONG");
    delay(5000); //5초

    lcd.clear();
    lcd.print("THIS STOP IS"); 
    lcd.setCursor(0,1);
    lcd.print("CHEONGNYANGNI");
    delay(5000); //5초  

    lcd.clear();
    lcd.print("THIS STOP IS"); 
    lcd.setCursor(0,1);
    lcd.print("HOEGI");
    delay(5000); //5초  
    
    for (int i=0 ; i<scoreLen ; i++)
    { int duration = 150;
    playNote(score[i], duration);
    }
    
    delay(1850);
    lcd.clear();
    lcd.print("THIS STOP IS"); 
    lcd.setCursor(0,1);
    lcd.print("HUFS");
    delay(20000);
    lcd.clear();
  }
}

 void playNote(char note, int duration){

  for (int i=0 ; i<noteCount ; i++)
  { if (noteNames[i] == note)
    tone(BuzzerPin, frequency[i], duration);
  }
    delay(duration);
}
