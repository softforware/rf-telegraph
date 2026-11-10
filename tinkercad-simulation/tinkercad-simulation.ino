#include <LiquidCrystal.h>

// ===== LCD PINS =====
// RS = 12
// EN = 10
// D4 = 5
// D5 = 4
// D6 = 3
// D7 = 2
LiquidCrystal lcd(12, 10, 5, 4, 3, 2);

// Recording Morse code data
String code = "";
String english = " ";
int duration = 0;
bool pressed = false;
bool pause = false;
int dot = 1;

String key = " .- -... -.-. -.. . ..-. --. .... .. .--- -.- .-.. -- -. --- .--. --.- .-. ... - ..- ...- .-- -..- -.-- --.. ";
String alphabet = "?aa bbbb cccc ddd e ffff ggg hhhh ii jjjj kkk llll mm nn ooo pppp qqqq rrr sss t uuu vvvv www xxxx yyyy zzzz";

void setup() {
  Serial.begin(9600);

  pinMode(7, INPUT);
  pinMode(6, INPUT);

  // ===== LCD Setup =====
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Morse System");
  lcd.setCursor(0, 1);
  lcd.print("Ready...");
  delay(1200);
  lcd.clear();
}

void loop() {

  // ---- Button Pressed ----
  if (digitalRead(7) == HIGH) {

    if (!pressed) {
      duration = 0;
      pause = false;
    }
    pressed = true;
  }

  // ---- Button Released → Dot or Dash ----
  else if (pressed) {

    if (duration >= 3 * dot) {
      code.concat("-");
    } else {
      code.concat(".");
    }

    duration = 0;
    pressed = false;
  }

  // ---- End of Letter ----
  else if (duration >= 3 * dot && code.length() > 1) {

    code.concat(" ");

    english.concat(alphabet.substring(key.indexOf(code) + 1, key.indexOf(code) + 2));

    // LCD displays the growing message
    lcd.clear();
    lcd.print(english.substring(max(0, (int)english.length() - 16))); // auto-scroll like behavior

    code = " ";
    Serial.println(english);
  }

  // ---- Space Between Words ----
  else if (duration > 7) {

    english = english + " ";

    lcd.clear();
    lcd.print(english.substring(max(0, (int)english.length() - 16)));

    duration = 0;
    pause = true;
  }

  // ---- Increase Duration ----
  if (!pause) {
    duration = duration + 1;
  }

  // ---- Reset if A1 Triggered ----
  if (digitalRead(6) == HIGH) {
    english = "";
    lcd.clear();
  }

  delay(100);
}
