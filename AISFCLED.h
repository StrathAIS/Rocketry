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

int NUM_LEDS = 1;           // Change this for how many LED's are on a each strand
#define COLOR_ORDER NEO_RGBW  // NeoPixel color format (see Adafruit_NeoPixel)
//int8_t megaPin1 = 9;
//int8_t megaPin2 = 8;
//int8_t megaPin3 = 10;
//Adafruit_NeoPixel megaAltitude(NUM_LEDS, -1, COLOR_ORDER);      
//Adafruit_NeoPixel megaStatus(NUM_LEDS, -1, COLOR_ORDER);
//Adafruit_NeoPixel megaCabinLight(NUM_LEDS, -1, COLOR_ORDER);
//
//
static uint32_t cRed = Adafruit_NeoPixel::Color(255, 0, 0, 0);
static uint32_t cOrange = Adafruit_NeoPixel::Color(255, 160, 0, 0);
static uint32_t cCyan = Adafruit_NeoPixel::Color(0, 255, 255, 0);
static uint32_t cGreen = Adafruit_NeoPixel::Color(0, 255, 0, 0);
static uint32_t cYellow = Adafruit_NeoPixel::Color(255, 255, 0, 0);
static uint32_t cBlue = Adafruit_NeoPixel::Color(0, 0, 255, 0);
static uint32_t cPurple = Adafruit_NeoPixel::Color(192, 0, 255, 0);
static uint32_t cMagenta = Adafruit_NeoPixel::Color(255, 0, 255, 0);
static uint32_t cWhite = Adafruit_NeoPixel::Color(0, 0, 0, 255);
static uint32_t cOff = Adafruit_NeoPixel::Color(0, 0, 0, 0);
//
//
bool ledInit(Adafruit_NeoPixel led_indicator);
void setAltColour(float alt, float sAlt, Adafruit_NeoPixel& megaAltitude);
//
//
bool ledInit(Adafruit_NeoPixel led_indicator)
{
    led_indicator.begin();
    led_indicator.setBrightness(40);
    for (int i = 0; i < 4; i++) {
        led_indicator.setPixelColor(0, cRed);
        led_indicator.show();
        delay(25);
        led_indicator.setPixelColor(0, cOff);
        led_indicator.show();
        delay(25);
    }
    return true;
}
//
//
void setAltColour(float cAlt, float zAlt, Adafruit_NeoPixel& mAlt) {
    /*
        The altitude has to cover the range of roughly 0 to 10,000ft
        Altitude reached above this is very(!!!) likely
        So, what do: run from red to green between 0 and 3000m
        FLIP the colour at 10000ft to white fading to blue 
    */
    if (cAlt > 3048) // <- correct this number to be accurate to 10,000 ft in m
    {
        uint8_t r = 255, g = 0, b = 0, w = 0;
        int offset = (cAlt - zAlt) / 250;
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
    if (cAlt <= 3048)
    {
        uint8_t r = 0, g = 0, b = 0, w = 255;
        int offset = (cAlt - zAlt) / 250;
        //Serial.print("Offset from Start: ");
        Serial.println(offset);
        if (w + offset > 255 && b - offset < 0) {
            mAlt.setPixelColor(0, Adafruit_NeoPixel::Color(r, g, b, w));
        }
        uint8_t cR = r, cG = g, cB = b - offset, cW = w + offset;
        if (w + offset < 255 && b - offset > 0) {
            mAlt.setPixelColor(0, Adafruit_NeoPixel::Color(cR, cG, cB, cW));
        }
    }
}

#endif