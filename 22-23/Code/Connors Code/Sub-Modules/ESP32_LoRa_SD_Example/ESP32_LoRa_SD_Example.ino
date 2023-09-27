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

bool SDfail =0;


void setup() 
{

  Serial.begin(115200);
  setupSD();

  
}




void loop() {
  // put your main code here, to run repeatedly:


appendFile(SD, "/data.txt", "awrite");
  
delay(1000);
}



void setupSD() {
  
  Serial.println("Initializing SD card...");
sdSPI.begin(SD_CLK, SD_MISO, SD_MOSI, SD_CS);
if (!SD.begin(SD_CS, sdSPI)) {

Serial.println("Card Mount Failed");

SDfail=1;

}

else

{
Serial.println("Success");
SDfail = 0;
}
uint8_t cardType = SD.cardType();
if(cardType == CARD_NONE) {
  Serial.println("No SD card attached");

  SDfail=1;

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
    SDfail = 1;
    return;
  }
  if(file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
    SDfail = 1;
  }
  file.close();
}


void appendFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if(!file) {
    Serial.println("Failed to open file for appending");
    SDfail = 1;
    return;
  }
  if(file.print(message)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
    SDfail = 1;
  }
  file.close();
}



void logSDCard() {
//  dataMessage = String(Vbat_filter) + "," + String(Iout_avg) + "," + String(power)+ "\r\n";
 // Serial.print("Save data: ");
 // Serial.println(dataMessage);
 // appendFile(SD, "/data.txt", dataMessage.c_str());
}
