#include <LiquidCrystal.h>
#include <math.h>

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

const int ledPins[] = {53, 52, 51, 47, 50, 49, 48};
const int potPin = A0;
const int buttonPin = A1; 

void setup() {
  randomSeed(analogRead(0)); 
  for (int i = 0; i < 7; i++) {
    pinMode(ledPins[i], OUTPUT); 
  }
  pinMode(buttonPin, INPUT_PULLUP);
  lcd.begin(16, 2);
}

void loop() {
  int potValue = analogRead(potPin);
  delay(10); 

  int rolls = constrain(map(potValue, 0, 1023, 1, 99), 1, 99);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Rolls: ");
  lcd.print(rolls);
  lcd.setCursor(0, 1);
  lcd.print("Press Button      ");

  if (digitalRead(buttonPin) == LOW) {
    while (digitalRead(buttonPin) == LOW); 
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Rolling...");

    int longestStreak = 0, streak = 1, previousRoll = 0, currentRoll;
    for (int i = 0; i < rolls; i++) {
      currentRoll = random(1, 7);
      int dynamicDelay = max(50, 200 - (i * 2));

      showDice(currentRoll, dynamicDelay); 

      if (currentRoll == previousRoll) {
        streak++;
      } else {
        streak = 1;
        previousRoll = currentRoll;
      }

      if (streak > longestStreak) {
        longestStreak = streak;
      }
    }

    double probability = pow(1.0 / 6.0, longestStreak) * 100;

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("LngStk: ");
    lcd.print(longestStreak);
    lcd.setCursor(0, 1);
    lcd.print("Prob: ");
    lcd.print(probability, 2);
    lcd.print("%");

    delay(5000); 
  }
}

void showDice(int number, int dynamicDelay) {
  for (int i = 0; i < 7; i++) {
    digitalWrite(ledPins[i], LOW);
  }

  switch (number) {
    case 1:
      digitalWrite(ledPins[3], HIGH);
      break;
    case 2:
      digitalWrite(ledPins[0], HIGH);
      digitalWrite(ledPins[6], HIGH);
      break;
    case 3:
      digitalWrite(ledPins[0], HIGH);
      digitalWrite(ledPins[3], HIGH);
      digitalWrite(ledPins[6], HIGH);
      break;
    case 4:
      digitalWrite(ledPins[0], HIGH);
      digitalWrite(ledPins[2], HIGH);
      digitalWrite(ledPins[4], HIGH);
      digitalWrite(ledPins[6], HIGH);
      break;
    case 5:
      digitalWrite(ledPins[0], HIGH);
      digitalWrite(ledPins[2], HIGH);
      digitalWrite(ledPins[4], HIGH);
      digitalWrite(ledPins[6], HIGH);
      digitalWrite(ledPins[3], HIGH);
      break;
    case 6:
      digitalWrite(ledPins[0], HIGH);
      digitalWrite(ledPins[1], HIGH);
      digitalWrite(ledPins[2], HIGH);
      digitalWrite(ledPins[4], HIGH);
      digitalWrite(ledPins[5], HIGH);
      digitalWrite(ledPins[6], HIGH);
      break;
  }

  delay(dynamicDelay); 
  }
