// ESP32 C3 SERIAL1 (second UART)
HardwareSerial mySerial1(1);

int rxPin = 20;
int txPin = 21;

String devAddr = ""; //INSERT VALUES
String nwkkey = ""; //INSERT VALUES
String appskey = ""; //INSERT VALUES

#include <SoftwareSerial.h>
SoftwareSerial myserial(1, 0);


void setup()
{
  Serial.begin(115200);
  delay(1000);
 Serial.println("ABP test");
  
  pinMode(txPin, OUTPUT);
  pinMode(rxPin, INPUT);

  pinMode(10, OUTPUT); //Rak enable
  pinMode(4, OUTPUT); // LED

    // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  myserial.begin(9600);

   Serial.println("SerialOK");

    digitalWrite(10, HIGH); // Switch on RAK
    delay(3000);
  mySerial1.begin(115200, SERIAL_8N1, rxPin, txPin);
  delay(1000);    

   Serial.println("Setup at command");
 
  mySerial1.println("AT+NWM=1"); // Set LoRaWan mode
  delay(300); 
   mySerial1.println("AT+NJM=0"); // Set ABP
  delay(300);
  mySerial1.println("AT+BAND=4");// Set EU868 frequency band
  delay(300);
  Serial.println("AT+DR=5 // Set EU868 frequency band");// Set EU868 frequency band  
  delay(300); 
   mySerial1.printf("AT+DEVADDR=");
  mySerial1.println(devAddr);
  delay(300);
  mySerial1.printf("AT+NWKSKEY=");
  mySerial1.println(nwkkey);
  delay(300);  
  mySerial1.printf("AT+APPSKEY=");
  mySerial1.println(appskey);
  delay(300);
   if (mySerial1.available())
  { // If anything comes in Serial1 (pins 4 & 5)
    while (mySerial1.available())
      Serial.write(mySerial1.read()); // read it and send it out Serial (USB)
  }

  delay(1000);
    digitalWrite(4, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(4, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);  
}

void loop()
{

if ((myserial.available())){
  int code = 100*myserial.read();
 // int code = code_char.toInt();
  delay(200);
unsigned char mydata[4];
mydata[0] = 0x1; // CH1
mydata[1] = 0x3; // Temp
mydata[2] = code >> 8;
mydata[3] = code & 0xFF;

char str[32] = "";
array_to_string(mydata, 4, str);

mySerial1.printf("AT+SEND=3:");
mySerial1.println(str);
   delay(5300);
  if (mySerial1.available())
  { // If anything comes in Serial1 (pins 4 & 5)
    while (mySerial1.available())
      Serial.write(mySerial1.read()); // read it and send it out Serial (USB)
  }
  Serial.println("AT set complete with downlink");
  Serial.println("Uplink");
  }
}



void array_to_string(byte array[], unsigned int len, char buffer[])
{
    for (unsigned int i = 0; i < len; i++)
    {
        byte nib1 = (array[i] >> 4) & 0x0F;
        byte nib2 = (array[i] >> 0) & 0x0F;
        buffer[i*2+0] = nib1  < 0xA ? '0' + nib1  : 'A' + nib1  - 0xA;
        buffer[i*2+1] = nib2  < 0xA ? '0' + nib2  : 'A' + nib2  - 0xA;
    }
    buffer[len*2] = '\0';
}

// Return temperature level in degree
 float measure_temp(){

flush_serial_AT();// flush AT Serial reading buffer
  
mySerial1.println("ATC+TEMP=?"); // Request bat value
 String temperature;
 delay(300);

 if(mySerial1.available()){
        temperature = mySerial1.readStringUntil('\n');
        Serial.print("Temperature:");
        Serial.println(temperature);
 }
 
return temperature.toFloat();
 }

void flush_serial_AT(){

   if (mySerial1.available())
  { // If anything comes in Serial1 (pins 4 & 5)
    while (mySerial1.available())
      Serial.write(mySerial1.read()); // read it and send it out Serial (USB)
  }
  delay(100);
}
