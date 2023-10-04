/**************************************************************************
 This is an example for our Monochrome OLEDs based on SSD1306 drivers

 Pick one up today in the adafruit shop!
 ------> http://www.adafruit.com/category/63_98

 This example is for a 128x64 pixel display using I2C to communicate
 3 pins are required to interface (two I2C and one reset).

 Adafruit invests time and resources providing this open
 source code, please support Adafruit and open-source
 hardware by purchasing products from Adafruit!

 Written by Limor Fried/Ladyada for Adafruit Industries,
 with contributions from the open source community.
 BSD license, check license.txt for more information
 All text above, and the splash screen below must be
 included in any redistribution.
 **************************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int pressure_BMP388=0;
int avg_alt_BMP388=0;


void printParameters();

void setup() {
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

//  // Show initial display buffer contents on the screen --
//  // the library initializes this with an Adafruit splash screen.
//  display.display();
//  delay(2000); // Pause for 2 seconds
//
//  // Clear the buffer
//  display.clearDisplay();
//
//  // Draw a single pixel in white
//  display.drawPixel(10, 10, SSD1306_WHITE);

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
//  display.display();
//  delay(2000);
  // display.display() is NOT necessary after every single drawing command,
  // unless that's what you want...rather, you can batch up a bunch of
  // drawing operations and then update the screen all at once by calling
  // display.display(). These examples demonstrate both approaches...


   display.clearDisplay();
//
//  display.setTextSize(1); // Draw 2X-scale text
//  display.setTextColor(SSD1306_WHITE);
//  display.setCursor(0, 10);
//  display.print("280(mPa)=");
//  display.setCursor(0, 20);
//  display.print("280(m) =");
//  display.setCursor(0, 40);
//  display.print("388(mPa)=");
//  display.setCursor(0, 50);
//  display.print("388(m)=");

  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
//  display.setCursor(90, 20);
//  display.print("hPa");
//
//  display.setCursor(90, 40);
//  display.print("m");

//  display.setCursor(0, 40);
//  display.print("(m)");
//
//  display.setCursor(0, 50);
//  display.print("(mPa)");





  
  
//  display.print("Pressure (mPa) |");
//  display.print("Height (m)");
//  display.display();


  

}


int c = 0;

void loop() {

  display.clearDisplay();
  printParameters();
  display.setCursor(0, 20);
  display.print(avg_alt_BMP388);
  

  display.setCursor(0, 40);
  display.print(pressure_BMP388);
  display.display();

  avg_alt_BMP388++;
  pressure_BMP388++;



  
}




void printParameters()

{
  
  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(90, 20);
  display.print("hPa");

  display.setCursor(90, 40);
  display.print("m");
  

}
