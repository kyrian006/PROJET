/*
  AnalogReadSerial

  Reads an analog input on pin 0, prints the result to the Serial Monitor.
  Graphical representation is available using Serial Plotter (Tools > Serial Plotter menu).
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/AnalogReadSerial
*/
#include <SoftwareSerial.h>

SoftwareSerial myserial(1, 0);

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  myserial.begin(9600);

   Serial.println("test");
}
  

// the loop routine runs over and over again forever:
void loop() {
  
  if ((myserial.available())){
  Serial.println(myserial.read());
  delay(200);
}
}
