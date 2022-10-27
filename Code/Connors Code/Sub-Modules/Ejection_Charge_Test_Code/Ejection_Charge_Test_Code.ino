// Ejection Charge Test Code
/*
 * 
 *  In the serial monitor, enter the letter 'd' to send an ignition to the drogue, and 'm' to fire to main
 * 
 * 
 * Only pins 4 and 0 work for Digital GPIO
 */

//#define pinDrogue 4
//#define pinMain  0


//Wroom Test Board
#define pinDrogue 13
#define pinMain  0

void setup() {
  // put your setup code here, to run once:
 Serial.begin(9600);
 Serial.println("Ejection Charge Test Code Begin");

 //Setup Pins

 pinMode(pinDrogue, OUTPUT);
 pinMode(pinMain, OUTPUT);
  

}

void loop() {
  // put your main code here, to run repeatedly:

  //
  while(Serial.available())
  {
    char incomingChar = Serial.read();

    if (incomingChar == 'd')

    {
      Serial.println( "drogue fired");
      digitalWrite(pinDrogue, HIGH);
      delay(300);
      digitalWrite(pinDrogue, LOW);
      
    }
    else if (incomingChar == 'm')
    {

      Serial.println( "main fired");
      digitalWrite(pinMain, HIGH);
      delay(300);
      digitalWrite(pinMain, LOW);
      

    }
    
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
  

}
