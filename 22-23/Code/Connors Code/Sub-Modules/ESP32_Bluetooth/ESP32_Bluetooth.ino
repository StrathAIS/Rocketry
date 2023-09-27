//This example code is in the Public Domain (or CC0 licensed, at your option.)
//By Evandro Copercini - 2018
//
//This example creates a bridge between Serial and Classical Bluetooth (SPP)
//and also demonstrate that SerialBT have the same functionalities of a normal Serial

#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

bool BT_data_collected = false;
char BT_message[5][30] = {"hello","there","how","yadoin\n"};     // [Qparam][charnum]
int BT_message_num=0;
int BT_num_messages = 1;
int charnum = 0;
char incomingChar;


///BT_send(num_messages,BT_message);
//BT_receive();



void BT_send(int BT_num_messages)//, char BT_message[5][30])

{

  Serial.println(BT_message_num);
  
  
  for (BT_message_num=0 ;BT_message_num < 5;BT_message_num++) // increment through each message
  {

    
    while(BT_message[BT_message_num][charnum] != '\n')  // send each character
    {
      
    //SerialBT.write(BT_message[BT_message_num][charnum]);
    Serial.print(BT_message[BT_message_num][charnum]);
    
    charnum++;
    
    }
    charnum=0;
  
  }
}


void setup() 
{
  Serial.begin(115200);
  SerialBT.begin("ESP32"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");

  BT_send(4);
}

void loop() {

/*

while(!BT_data_collected)   // Run until all data
{
  if (SerialBT.available()) 
  {

    incomingChar = SerialBT.read();
    int result = iscntrl(incomingChar);

     if (result==0) // If  not a 'hidden' control character, append to message
     {
     BT_message[BT_message_num][charnum] = incomingChar;
     }

     charnum++;

     if (incomingChar == '\n')
     {

       Serial.println(BT_message[BT_message_num]);
       BT_message_num++;
       charnum =0;

      if (BT_message[BT_message_num] =="done" || BT_message_num >=BT_num_messages ) // prevent buff overflow
      {
        BT_data_collected =true;
      }
      
     }
   
  }
  delay(20);

}

*/
}
