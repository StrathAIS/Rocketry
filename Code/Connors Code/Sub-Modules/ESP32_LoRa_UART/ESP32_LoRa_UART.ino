#include <SoftwareSerial.h>

//COM 19

// The serial connection to the GPS module
SoftwareSerial ss(25, 12);

int c=0;
void setup(){
  Serial.begin(9600);
  ss.begin(9600);
}

void loop(){
//  while (ss.available() > 0){
//    // get the byte data from the GPS
//    byte gpsData = ss.read();
//    Serial.write(gpsData);
//  }

ss.print("Gripper/Close\n");
delay(3000);
ss.print("Gripper/Open\n");
delay(2000);
  
}
