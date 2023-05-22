/*
  LED control header 

  This header is to control any LED's we may wish to use.
  Currently these are only really used for testing purposes.
  However we may wish to use them for cabin lighting of the avionics bay. 
  We may also find other uses for them in the future. 
  
  Created: 16th May 2023
  Last Update: 19th May 2023
  Created By: Michael Haggart 
  For: StarthAIS
  Updated by: Michael Haggart 
              #Add New Names Here
*/

#ifndef AISFCLED
#define AISFCLED

#include <Adafruit_NeoPixel.h>
#include <math.h>

#define NUM_LEDS 1            // Change this for how many LED's are on a each strand
#define COLOR_ORDER NEO_RGBW  // NeoPixel color format (see Adafruit_NeoPixel)
int8_t megaPin1 = 9;
int8_t megaPin2 = 8;
int8_t megaPin3 = 10;



Adafruit_NeoPixel megaAltitude(NUM_LEDS, megaPin1, COLOR_ORDER);
Adafruit_NeoPixel megaStatus(NUM_LEDS, megaPin2, COLOR_ORDER);
Adafruit_NeoPixel megaCabinLight(NUM_LEDS, megaPin3, COLOR_ORDER);

static uint32_t cRed     = Adafruit_NeoPixel::Color(255, 0, 0, 0);
static uint32_t cOrange  = Adafruit_NeoPixel::Color(255, 160, 0, 0);
static uint32_t cCyan    = Adafruit_NeoPixel::Color(0, 255, 255, 0);
static uint32_t cGreen   = Adafruit_NeoPixel::Color(0, 255, 0, 0);
static uint32_t cYellow  = Adafruit_NeoPixel::Color(255, 255, 0, 0);
static uint32_t cBlue    = Adafruit_NeoPixel::Color(0, 0, 255, 0);
static uint32_t cPurple  = Adafruit_NeoPixel::Color(192, 0, 255, 0);
static uint32_t cMagenta = Adafruit_NeoPixel::Color(255, 0, 255, 0);
static uint32_t cWhite   = Adafruit_NeoPixel::Color(0, 0, 0, 255);
static uint32_t cOff     = Adafruit_NeoPixel::Color(0, 0, 0, 0);

void ledInit();
void colourState(int s, Adafruit_NeoPixel pxl);
void setAltColour(float alt, float sAlt, Adafruit_NeoPixel& megaAltitude);
void setStateColour(float alt, float sAlt, Adafruit_NeoPixel& megaStatus);

void ledInit()
{
  megaAltitude.begin(); megaStatus.begin(); megaCabinLight.begin();
  megaAltitude.setBrightness(50); megaStatus.setBrightness(50); megaCabinLight.setBrightness(50);
  for (int i = 0; i < 10; i++) {
    megaAltitude.setPixelColor(0, cRed);
    megaStatus.setPixelColor(0, cCyan);
    megaCabinLight.setPixelColor(0,cWhite);
    megaStatus.show();
    megaAltitude.show();
    megaCabinLight.show();
    delay(50);
    megaAltitude.setPixelColor(0, cOff);
    megaStatus.setPixelColor(0, cOff);
    megaCabinLight.setPixelColor(0,cOff);
    megaAltitude.show();
    megaStatus.show();
    megaCabinLight.show();
    delay(50);
  }
  megaAltitude.setPixelColor(0, cRed);
  megaCabinLight.setPixelColor(0,cWhite);
  megaAltitude.show();
  megaCabinLight.show();

}
/*~~DEPRICATED, USE .setPixelColor instead~~*/
void colourState(int s, Adafruit_NeoPixel pxl) {
  switch (s) {
    case 0:
      pxl.clear();
      pxl.fill(cRed);
      pxl.show();
      return;
    case 1:
      pxl.clear();
      pxl.fill(cOrange);
      pxl.show();
      return;
    case 2:
      pxl.clear();
      pxl.fill(cYellow);
      pxl.show();
      return;
    case 3:
      pxl.clear();
      pxl.fill(cGreen);
      pxl.show();
      return;
    case 4:
      pxl.clear();
      pxl.fill(cCyan);
      pxl.show();
      return;
    case 5:
      pxl.clear();
      pxl.fill(cBlue);
      pxl.show();
      return;
    case 6:
      pxl.clear();
      pxl.fill(cPurple);
      pxl.show();
      return;
    case 7:
      pxl.clear();
      pxl.fill(cMagenta);
      pxl.show();
      return;
    case 8:
      pxl.clear();
      pxl.fill(cWhite);
      pxl.show();
      return;
    case 9:
      pxl.clear();
      pxl.fill(cOff);
      pxl.show();
      return;
  }
}

void setAltColour(float alt, float sAlt, Adafruit_NeoPixel& mAlt) {
  uint8_t r = 255, g = 0, b = 0, w = 0;
  int offset = (alt - sAlt) /250;
  //Serial.print("Offset from Start: ");
  Serial.println(offset);
  if (r + offset > 255 && g - offset < 0) {
    mAlt.setPixelColor(0, Adafruit_NeoPixel::Color(r, g, b, w));
  }
  uint8_t cR = r + offset, cG = g - offset, cB = b, cW = w;
  if (r + offset < 255 && g - offset > 0) {
    mAlt.setPixelColor(0, Adafruit_NeoPixel::Color(cR, cG, cB, cW));
  }
}

/*~~Something is wrong in this setStateColour code, can't pinpoint what
  ~~I think it's something to do with the detaD, and the if statements running repeatedly*/
void setStateColour(float alt, float sAlt, Adafruit_NeoPixel& mStatus) {
  int deltaD = sAlt - alt;
  Serial.print("Pressure Difference: ");
  Serial.println(deltaD);
  if (deltaD > sAlt - 69681) {
    Serial.println("10,000 feet!");
    mStatus.fill(cWhite);
    mStatus.show();
  }
  if (deltaD > sAlt - 76712) {
    Serial.println("7,500 feet!");
    mStatus.fill(cPurple);
    mStatus.show();
  }
  if (deltaD > sAlt - 84307) {
    Serial.println("5,000 feet!");
    mStatus.fill(cCyan);
    mStatus.show();
  }
  if (deltaD > sAlt - 89148) {
    Serial.println("3,500 feet!");
    mStatus.fill(cGreen);
    mStatus.show();
  }
  if (deltaD > sAlt - 94212) {
    Serial.println("2,000 feet!");
    mStatus.fill(cYellow);
    mStatus.show();
  }
  if (deltaD > sAlt - 97716) {
    Serial.println("1,000 feet!");
    mStatus.fill(cOrange);
    mStatus.show();
  }
  if (deltaD > sAlt - 99507) {
    Serial.println("500 feet!");
    mStatus.fill(cRed);
    mStatus.show();
  }
  if (deltaD < sAlt - 99507) {
    Serial.println("Waiting for major change");
    mStatus.fill(cOff);
    mStatus.show();
  }
}

#endif