#include <Keypad.h>
#include "SR04.h"
#include <LiquidCrystal.h>

#define BUZZER 13
#define RED 2
#define GREEN 3
#define BLUE 4
#define TRIG_PIN 52
#define ECHO_PIN 53
char passcode[6];
char hexaKeys[4][4] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
char set[6] = {'3', 'B', 'A', 'D', 'C', '2'};
byte rowPins[4] = {12, 11, 10, 9};
byte colPins[4] = {8, 7, 6, 5};
long a;
long b = 18;
int count = 0;
boolean s1 = false;
boolean s2 = false;
boolean initialPromptDisplayed = false; 
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, 4, 4);
LiquidCrystal lcd(26, 27, 25, 24, 23, 22);
SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);

 

void setup() {
  Serial.begin(9600);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  analogWrite(RED, 255);
  analogWrite(GREEN, 0);
  analogWrite(BLUE, 0);
  lcd.begin(16, 2);
}

void loop(){
    if(count == 0 && !initialPromptDisplayed){
    lcd.clear(); 
    lcd.print("1: Type Code");
    initialPromptDisplayed = true;
  }
  char customKey = customKeypad.getKey();
  if(customKey){
    initialPromptDisplayed = false;
    if(customKey == '#'){
      count = 0;
      lcd.clear();
      lcd.print("1: Type Code");
    }else{
      passcode[count] = customKey;
      count++;
      if(count >= 6){ 
        bool passcodeCorrect = true;
        for(byte i = 0; i < 6; i++){
          if(passcode[i] != set[i]){
            passcodeCorrect = false;
            break;
          }
        }
        
        if(passcodeCorrect){
          lcd.clear();
          lcd.print("Success!");
          analogWrite(GREEN, 244);
          analogWrite(BLUE, 79);
          delay(3000);
          s1 = true;
        }else{
          lcd.clear();
          lcd.print("Not Authenticated!");
          digitalWrite(BUZZER, HIGH);
          exit(0);
        }
      }
    }
  }

  if(s1){
    lcd.clear();
    lcd.print("2: Dist Test");
    delay(2500); 
    for(int i = 0; i < 5; i++){
      lcd.clear();
      lcd.print(String(5-i));
      delay(1000);
      lcd.clear();
    }
    for(byte i = 0; i < 10; i++){
      a=sr04.Distance();
      if((a > (b+3)) or (a < (b-3))){
          lcd.clear();
          lcd.print("Not Authenticated!");
          digitalWrite(BUZZER, HIGH);
          exit(0);
      }
      lcd.clear();
      lcd.print("Testing");
      delay(100);
      lcd.clear();
      lcd.print("Testing.");
      delay(100);
      lcd.clear();
      lcd.print("Testing..");
      delay(100);
      lcd.clear();
      lcd.print("Testing...");
      delay(100);
   
    }
    lcd.clear();
    lcd.print("Welcome Back!");
    analogWrite(GREEN, 255);
    analogWrite(RED, 0);
    analogWrite(BLUE, 0);
    exit(0);
    
  }
}
