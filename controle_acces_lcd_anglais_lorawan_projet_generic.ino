#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <Key.h>
#include <Keypad.h>
#include <Wire.h>
#include <SoftwareSerial.h>
const int ROW_NUM = 4; //four rows
const int COLUMN_NUM = 4; //four columns
char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3', 'A'},
  {'4','5','6', 'B'},
  {'7','8','9', 'C'},
  {'*','0','#', 'D'}
};
SoftwareSerial connexionserie(12, 13);
byte pin_rows[ROW_NUM] = {10, 9, 8, 7}; //connect to the row pinouts of the keypad
byte pin_column[COLUMN_NUM] = {6, 5, 4, 3}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );
String inputkey = "";
String clesecrete = ""; //INSERT SECRET MASTERKEY
String masterkey = ""; // INSERT SECRET MASTERKEY
bool modemaitre = false;
LiquidCrystal_I2C lcd(0x27,  16, 2);
byte cadenas[8] = {
	0b01110,
	0b10001,
	0b10001,
	0b11111,
	0b11011,
	0b11011,
	0b11111,
	0b00000
};
Servo moteur;
void setup(){
  Serial.begin(9600);
  connexionserie.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, cadenas);
  moteur.attach(11);
  connexionserie.begin(9600);
  connexionserie.write(6);
}

void loop(){
if (modemaitre == false)  {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Locked");
  lcd.setCursor(0,1);
  lcd.print("Please Wait...");
  delay(3000);
  entreecode();
  }
  else {
    modemaitrefunc();
  }
}

void resetfunc() {
    inputkey = "";
    lcd.setCursor(0,1);
    lcd.print("                ");
}
void modemaitrefunc() {
    modemaitre:
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("* Lock");
    lcd.setCursor(0,1);
    lcd.print("# New code");
    while (true) {
    char key = keypad.getKey();
      if (key == '*') {
        modemaitre = false;
        lcd.clear();
        lcd.print("Locking...");
        moteur.write(180);
        connexionserie.write(2);
        delay(2000);
        break;
        }
      else if (key == '#') {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("New code : ");
        while (true) {
          char key = keypad.getKey();
          if (key) {
            if (key == 'A') {
              resetfunc();
              }
            else if (key == 'D') {
              lcd.clear();
              clesecrete = inputkey;
              lcd.print("Saving...");
              connexionserie.write(4);
              delay(500);
              inputkey = "";
              goto modemaitre;
             }
            else if (inputkey.length() >= 15 ) {
          }
          else {
            lcd.setCursor(0,1);
            inputkey += key;
            lcd.print(inputkey);
            }
          }
        }
      }
  }
}
void entreecode() {
  entreecode:
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Dial the code");
  Serial.print(clesecrete);
  while (true) {
    char key = keypad.getKey();
    if (key) {
      if (key == 'A') {
        resetfunc();
        }
      else if (key == 'D') {
        if (clesecrete == inputkey || inputkey == masterkey) {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Valid Code");
          lcd.setCursor(0,1);
          lcd.print("Access Allowed");
          connexionserie.write(1);
          // 1 unlocked 2 locked 3 false try print to rf
          delay(2000);
          moteur.write(0);
          modemaitre = true;
          inputkey = "";
          break;
          }
  
        else {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Invalid Code");
          lcd.setCursor(0,1);
          lcd.print("Access Denied");
          connexionserie.write(3);
          inputkey = "";
          delay(2000);
          goto entreecode;
          }
          ;
        } 
    else if (inputkey.length() >= 15 ) {
      }   
    else {
        lcd.setCursor(0,1);
        inputkey += key;
        lcd.print(inputkey);
        
        }
      }
    }
}
