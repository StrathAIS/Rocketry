// Vacuum Chamber Test
#include <stdio.h> 
// BMP388
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"

#define SEALEVELPRESSURE_HPA (1005)

Adafruit_BMP3XX bmp;

// OLED
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// SD CARD

#include "FS.h"
#include "SD.h"
#include "SPI.h"


#define SD_CS 13
#define SD_CLK 14
#define SD_MOSI 15
#define SD_MISO 2

SPIClass sdSPI(HSPI);

void setupSD(void);
void writeFile(fs::FS &fs, const char,const char);
void appendFile(fs::FS &fs, const char , const char);
void logSDCard(void);


int offset = 0;
short state = 1;
float secs =0;
float timestep = 0.1;
float secs_state_trigger = 0;
int alt_bef;

//States
/*
 *  1 = Idle
 *  2 = Boost/Launch
 *  3 = Apogee
 *  4 = Main
 *  5 = Land
 */

void setup() {
  // put your setup code here, to run once:
 Serial.begin(115200);
 
 setupSD();
    
   if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

     display.clearDisplay();

 if (!bmp.begin_I2C()) {   // hardware I2C mode, can pass in address & alt Wire
  //if (! bmp.begin_SPI(BMP_CS)) {  // hardware SPI mode  
  //if (! bmp.begin_SPI(BMP_CS, BMP_SCK, BMP_MISO, BMP_MOSI)) {  // software SPI mode
    Serial.println("Could not find a valid BMP3 sensor, check wiring!");
    while (1);
  }

  
  // Set up oversampling and filter initialization
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_50_HZ);

  int alt_sum=0;
  
  for (int i=0;i<10;i++)
  {
    alt_sum +=bmp.readAltitude(SEALEVELPRESSURE_HPA);
    
  }
  offset = alt_sum/9;

  Serial.print("Offset = ");
  Serial.print(offset);
  
  
}

int pressure,alt;
char pressure_c[10], alt_c[10];
char csv_buffer[20];

char secs_c[10], state_c[5];

void loop() {
  // put your main code here, to run repeatedly:
   if (! bmp.performReading()) {
    Serial.println("Failed to perform reading :(");
    return;
  }
  else
  {
    pressure = bmp.pressure / 100.0;
    alt = bmp.readAltitude(SEALEVELPRESSURE_HPA);
    
  }
  
  display.clearDisplay();

  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(90, 20);
  display.print("hPa");

  display.setCursor(100, 40);
  display.print("m");
    
    display.setCursor(0, 20); // Display pressure
    display.print(pressure);
   
  
    display.setCursor(0, 40);
    display.print(alt);
    display.display();

//    sprintf(pressure_c, "%g", pressure);
//    sprintf(alt_c, "%g", alt);

    sprintf(pressure_c, "%d", pressure);
    sprintf(alt_c, "%d", alt);
    sprintf(secs_c, "%f", secs);
    sprintf(state_c, "%d", state);

    Serial.println(pressure_c);
    Serial.println(alt_c);
    Serial.println();

    strcpy(csv_buffer, "");
    strcat(csv_buffer, secs_c);
    strcat(csv_buffer, ",");
    strcat(csv_buffer, state_c);
    strcat(csv_buffer, ",");
    strcat(csv_buffer, pressure_c);
    strcat(csv_buffer, ",");
    strcat(csv_buffer, alt_c);

    appendFile(SD, "/data.txt", csv_buffer); // Cant include a logSD function as GPS outputs lat=0 lon=0
    appendFile(SD, "/data.txt", "\r\n"); // Cant include a logSD function as GPS outputs lat=0 lon=0

    
    delay(100);  // timestep in milliseconds
    secs = secs +timestep;   // timestep = 0.1 seconds
//    Serial.println(state);
//    Serial.println(alt);
//    Serial.println();

    // State Detection

    // Compare height reading to reading 1.5s before and if large enough, rocket launched
    if (secs_state_trigger < 1.5)
    {
      secs_state_trigger += timestep;
      
    }
    else // if reached 1.5 secs

    {
    
      if ((alt - alt_bef >20) && state ==1 )         // if reached 1.5 seconds, if height now - height before
      {
  
        state = 2;// Launched
        
        
      }
       // If a continual drop in altitude over 3 seconds
      else if ((alt < alt_bef) && state==2)
      {
  
        state = 3; // apogee
        
      }
      else if( (alt < 427) && state ==3)
      {
  
        state = 4; // main
  
        
      }
      else if (alt >=-40 && alt <30 && state==4)
      {
        state = 5; //landed
        
      }

      Serial.println("1.5s");
      alt_bef = alt;
      secs_state_trigger=0;
      
  
    }
    


}









void setupSD() {
  
  Serial.println("Initializing SD card...");
sdSPI.begin(SD_CLK, SD_MISO, SD_MOSI, SD_CS);
if (!SD.begin(SD_CS, sdSPI)) {

Serial.println("Card Mount Failed");


}

else

{
Serial.println("Success");

}
uint8_t cardType = SD.cardType();
if(cardType == CARD_NONE) {
  Serial.println("No SD card attached");



}

File file = SD.open("/data.txt");
  if(!file) {
    Serial.println("File doens't exist");
    Serial.println("Creating file...");
    writeFile(SD, "/data.txt", "Vbat, Iin, Iout \r\n");
  }
  else {
    Serial.println("File already exists");  
  }
  file.close();



}


void writeFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file) {
    Serial.println("Failed to open file for writing");
    
    return;
  }
  if(file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
    
  }
  file.close();
}


void appendFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if(!file) {
    Serial.println("Failed to open file for appending");
    
    return;
  }
  if(file.print(message)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
    
  }
  file.close();
}



void logSDCard() {

}
