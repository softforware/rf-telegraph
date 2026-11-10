#include <RH_ASK.h>
#include <SPI.h>
#include <LiquidCrystal.h>

// LCD pins: RS=12, E=10, D4=5, D5=4, D6=3, D7=2
LiquidCrystal lcd(12, 10, 5, 4, 3, 2);

String key = " .- -... -.-. -.. . ..-. --. .... .. .--- -.- .-.. -- -. --- .--. --.- .-. ... - ..- ...- .-- -..- -.-- --.. ";
String alphabet = "?aa bbbb cccc ddd e ffff ggg hhhh ii jjjj kkk llll mm nn ooo pppp qqqq rrr sss t uuu vvvv www xxxx yyyy zzzz";
String english = "";

RH_ASK driver;
String code = "";

void setup() {
  Serial.begin(9600);

  lcd.begin(16, 2);
  lcd.print("Starting...");

  if (!driver.init()) {
    Serial.println("init failed");
    lcd.setCursor(0, 1);
    lcd.print("Init failed   ");
  }

  pinMode(LED_BUILTIN, OUTPUT);

  // No autoscroll anywhere
  //lcd.noAutoscroll();
}

// Prints text, truncates if >16 chars
void displayScrolling(String text) {
  lcd.clear();
  lcd.setCursor(0, 0);

  if (text.length() <= 16) {
    lcd.print(text);
    return;
  }

  lcd.autoscroll();
  lcd.print(text);
  lcd.noAutoscroll();
}


void loop() {
  uint8_t buf[1];
  uint8_t buflen = sizeof(buf);
  if (driver.recv(buf, &buflen)) {

    digitalWrite(LED_BUILTIN, HIGH);
    int morse = buf[0] - 48;

    if (morse == 0) {
      code.concat(".");
    }
    else if (morse == 1) {
      code.concat("-");
    }
    else if (morse == 2) {
      code.concat(" ");
      english.concat(alphabet.substring(
        key.indexOf(code) + 1,
        key.indexOf(code) + 2
      ));
      code = " ";
      Serial.println(english);
      //displayText(english);
      displayScrolling(english);
    }
    else if (morse == 3) {
      english += " ";
      Serial.println(english);
      //displayText(english);
      displayScrolling(english);
    }
    else if (morse == 4) {
      english = "";
      Serial.println(english);
      lcd.clear();
    }

    digitalWrite(LED_BUILTIN, LOW);
  }
}
