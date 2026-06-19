#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// LEDs
int leds[] = {11, 6, 8, 10, 12};

// Buttons
int buttons[] = {5, 2, 3, 4, 13};

int buzzer = 9;
int startButton = 7;

int score = 0;
int highScore = 0;
int level = 1;
int target = 5;

int roundTime = 3000;

bool gameStarted = false;

void setup() {
  lcd.init();
  lcd.backlight();

  pinMode(buzzer, OUTPUT);
  pinMode(startButton, INPUT_PULLUP);

  for (int i = 0; i < 5; i++) {
    pinMode(leds[i], OUTPUT);
    pinMode(buttons[i], INPUT_PULLUP);
  }

  // صوت بداية
  tone(buzzer, 1000);
  delay(300);
  noTone(buzzer);

  lcd.print("Press Start");

  // ترتيب اللمبات
  digitalWrite(11, HIGH); delay(700); digitalWrite(11, LOW);
  digitalWrite(6, HIGH);  delay(700); digitalWrite(6, LOW);
  digitalWrite(8, HIGH);  delay(700); digitalWrite(8, LOW);
  digitalWrite(10, HIGH); delay(700); digitalWrite(10, LOW);
  digitalWrite(12, HIGH); delay(700); digitalWrite(12, LOW);

  randomSeed(analogRead(A0));
}

void loop() {

  // Start button
  if (!gameStarted) {
    if (digitalRead(startButton) == LOW) {

      gameStarted = true;

      tone(buzzer, 1500); delay(150);
      tone(buzzer, 2000); delay(150);
      noTone(buzzer);

      score = 0;
      level = 1;
      target = 5;
      roundTime = 3000;

      lcd.clear();
      lcd.print("Score: 0");
      delay(500);
    }
  }

  if (gameStarted) {

    int r = random(0, 5);

    digitalWrite(leds[r], HIGH);

    bool hit = false;
    unsigned long startTime = millis();

    while (millis() - startTime < roundTime) {

      if (digitalRead(buttons[r]) == LOW) {
        hit = true;
        break;
      }

      // غلط
      for (int i = 0; i < 5; i++) {
        if (i != r && digitalRead(buttons[i]) == LOW) {

          tone(buzzer, 300);
          delay(150);
          noTone(buzzer);

          score--;

          // 💀 Game Over
          if (score < -3) {

            if (score > highScore) {
              highScore = score;
            }

            lcd.clear();
            lcd.print("Game Over");

            delay(1500);

            lcd.clear();
            lcd.print("High Score:");
            lcd.setCursor(0,1);
            lcd.print(highScore);

            delay(3000);

            gameStarted = false;
            lcd.clear();
            lcd.print("Press Start");

            return;
          }
        }
      }
    }

    digitalWrite(leds[r], LOW);

    if (hit) {
      score++;

      tone(buzzer, 1200); delay(100);
      tone(buzzer, 1800); delay(100);
      noTone(buzzer);

      // Level system
      if (score >= target) {

        level++;

        tone(buzzer, 1500); delay(100);
        tone(buzzer, 2000); delay(100);
        noTone(buzzer);

        target += 5;
        roundTime -= 300;

        if (roundTime < 800) roundTime = 800;

        lcd.clear();
        lcd.print("Level ");
        lcd.print(level);
        delay(1000);
      }
    }

    // تحديث High Score
    if (score > highScore) {
      highScore = score;
    }

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Score: ");
    lcd.print(score);

    lcd.setCursor(0,1);
    lcd.print("Level:");
    lcd.print(level);
  }
}
