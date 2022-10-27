// Ejection Charge Test Code with Bluetooth
/*
 * 
 *  In the serial monitor, enter the letter 'd' to send an ignition to the drogue, and 'm' to fire to main
 * 
 * 
 * Only pins 4 and 0 work for Digital GPIO
 */

#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;


#define pinDrogue 4
#define pinMain  0

char incomingChar;

void setup() {
 
 //Serial Setup
 Serial.begin(9600);
 Serial.println("Ejection Charge Test Code Begin");

  //Bluetooth Setup
  SerialBT.begin("ESP32"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");


  delay(10000);
  SerialBT.print("Bluetooth Pairing Succesful");

 //Setup Pins

 pinMode(pinDrogue, OUTPUT);
 pinMode(pinMain, OUTPUT);
  

}

void loop() {
  // put your main code here, to run repeatedly:

  // Bluetooth

    if (SerialBT.available()) 
  {
    incomingChar = SerialBT.read();

    if(incomingChar=='d')
    {
      SerialBT.print("Drogue Fired");

      digitalWrite(pinDrogue, HIGH);
      delay(300);
      digitalWrite(pinDrogue, LOW);
      
    }
     else if(incomingChar=='m')
    {
      SerialBT.print("Main Fired");
      digitalWrite(pinMain, HIGH);
      delay(300);
      digitalWrite(pinMain, LOW);
      
    }

  }
  


  //
//  while(Serial.available())
//  {
//    char incomingChar = Serial.read();
//
//    if (incomingChar == 'd')
//
//    {
//      Serial.println( "drogue fired");
//      digitalWrite(pinDrogue, HIGH);
//      delay(300);
//      digitalWrite(pinDrogue, LOW);
//      
//    }
//    else if (incomingChar == 'm')
//    {
//
//      Serial.println( "main fired");
//      digitalWrite(pinMain, HIGH);
//      delay(300);
//      digitalWrite(pinMain, LOW);
//      
//
//    }
    
  }



//  digitalWrite(pinDrogue, HIGH);
//  digitalWrite(pinMain, HIGH);
//  
//  delay(1000);
//
//  digitalWrite(pinDrogue, LOW);
//  digitalWrite(pinMain, LOW);
//  
//  delay(1000);
  
