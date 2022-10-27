//#include <HardwareSerial.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h> 


TinyGPS gps; // create gps object 
float lat,lon; // create variable for latitude and longitude object  

//HardwareSerial GPS_Serial(1);
SoftwareSerial ss(25,12);

void setup() 
{
    Serial.begin(9600);
   // GPS_Serial.begin(9600, SERIAL_8N1, 25, 12);
   ss.begin(9600);
}

void loop()
{

  Serial.println(ss.read());
  
    while (ss.available() > 0) 
    {
      

      if(gps.encode(ss.read()))// encode gps data 
      {  
      gps.f_get_position(&lat,&lon); // get latitude and longitude 
    
      // display position 
      Serial.print("Position: "); 
      Serial.print("Latitude:"); 
      Serial.print(lat,6); 
      Serial.print(";"); 
      Serial.print("Longitude:"); 
      Serial.println(lon,6);  
      } 



      
//        uint8_t byteFromSerial = GPS_Serial.read();
//        Serial.print(byteFromSerial);
//        // Do something
    }
    
    //Write something like that
  //  MySerial.write(rand(0, 255));
}
