#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile

RH_ASK driver;

// pins
int LED=13;
//recording morse code data
String code="";
String english=" ";
int duration=0;
bool pressed=false;
bool pause=false;
int dot = 1;
String key= " .- -... -.-. -.. . ..-. --. .... .. .--- -.- .-.. -- -. --- .--. --.- .-. ... - ..- ...- .-- -..- -.-- --.. ";
String alphabet= "?aa bbbb cccc ddd e ffff ggg hhhh ii jjjj kkk llll mm nn ooo pppp qqqq rrr sss t uuu vvvv www xxxx yyyy zzzz";
void setup()
{

    Serial.begin(9600);	  // Debugging only
    if (!driver.init())
         Serial.println("init failed");
  
  pinMode(7,INPUT);
  pinMode(6, INPUT);
  pinMode(LED,OUTPUT);

}

void loop()
{
  //first time button is pressed, duration restarts
 
  if (digitalRead(7) == HIGH){
    
    digitalWrite(LED,HIGH);
    if (!pressed){
      duration=0;
      pause=false;
    }
    pressed=true;
  }
  //Once after button is released, . or - will be added
  //to code depending on duration
  else if (pressed){
    if (duration>=3*dot){
      code.concat("-");
      const char *msg = "1";
       driver.send((uint8_t *)msg, strlen(msg));
      driver.waitPacketSent();
    
    }
    else{
      code.concat(".");
      const char *msg = "0";
       driver.send((uint8_t *)msg, strlen(msg));
      driver.waitPacketSent();
    }
    duration=0;
    digitalWrite(LED,LOW);
    pressed=false;
  }
  //if the button hasn't been pressed for long enough, 
  //the code will be grouped as a letter in morseLetters
  else if (duration>=3*dot && code.length()>1){
    code.concat(" ");
    const char *msg = "2";
     driver.send((uint8_t *)msg, strlen(msg));
      driver.waitPacketSent();
     //Reciever, translate and clear code!!!!!!!!
    //english.concat(alphabet.substring(key.indexOf(code)+1,key.indexOf(code)+2));
    code=" ";
    Serial.println(english);
  }
  //if you wait even longer
  //, a space will be added to morseLetters.
  else if (duration>7){
    const char *msg = "3";
     driver.send((uint8_t *)msg, strlen(msg));
    driver.waitPacketSent();
    //RECIEVER, add a space
    //english=english+" ";
    duration=0;
    pause=true;
  }
  //duration increases every loop unless a space was just added
  if(!pause){
    duration=duration+1;
  }
  if(digitalRead(6) == HIGH){
    const char *msg = "4";
     driver.send((uint8_t *)msg, strlen(msg));
    driver.waitPacketSent();
  }
  delay(100);
}