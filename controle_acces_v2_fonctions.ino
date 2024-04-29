#include <Key.h>
#include <Keypad.h>
#include <EEPROM.h>
const int ROW_NUM = 4; //four rows
const int COLUMN_NUM = 4; //four columns

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3', 'A'},
  {'4','5','6', 'B'},
  {'7','8','9', 'C'},
  {'*','0','#', 'D'}
};

byte pin_rows[ROW_NUM] = {10, 9, 8, 7}; //connect to the row pinouts of the keypad
byte pin_column[COLUMN_NUM] = {6, 5, 4, 3}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );
String cle4 = "";
String clesecrete = "1234";
bool modemaitre = false;
void setup(){
  Serial.begin(9600);
}

void loop(){
if (modemaitre == false)  {
  Serial.println("Bienvenue, appuyez sur A pour réinitialiser la saisie ou D pour valider la saisie");
  Serial.println("Pour déverrouiller, veuillez entrer votre code secret : ");
  entreecode();
  }
  else {
    modemaitrefunc();
  }
}

void resetfunc() {
        cle4 = "";
        Serial.println("");
        Serial.println("Entrée réinitialisée");
        Serial.print("Entrez votre code secret : ");

}
void modemaitrefunc() {
    Serial.println("Mode Maître");
    Serial.println("Veuillez appuyer sur * pour vérrouiller ou # pour changer le code. ");
    while (true) {
    char key = keypad.getKey();
      if (key == '*') {
        modemaitre = false;
        Serial.println("Vérrouillage ...");
        delay(2000);
        break;
        }
      else if (key == '#') {
        Serial.println("Entrez le nouveau code secret : ");
        while (true) {
          char key = keypad.getKey();
          if (key) {
            if (key == 'A') {
              resetfunc();
              }
            else if (key == 'D') {
              Serial.println("Enregistrement ...");
              clesecrete = cle4;
              cle4 = "";
              Serial.println("Veuillez appuyer sur * pour vérrouiller ou # pour changer le code. ");
              break;
             }
          else {
            Serial.print(key);
            cle4 += key;
            }
          }
        }
      }
  }
}
void entreecode() {
  while (true) {
    char key = keypad.getKey();
    if (key) {
      if (key == 'A') {
        resetfunc();
        }
      else if (key == 'D') {
        if (cle4 == clesecrete) {
          Serial.println(" Clé valide, Accès autorisé");
          modemaitre = true;
          cle4 = "";
          break;
          }
        else {
          Serial.println(" Clé invalide, Accès refusé");
          Serial.print("Entrez votre code secret : ");
          cle4 = "";
          }
          ;
        } 
       
    else {
        Serial.print(key);
        cle4 += key;
        }
      }
    }
}