/*
  Barometer control header 

  This header is to provide a set of functions for use with the Barometer.
    **This is Mission Critical**
  The barometer will control deployment of shutes. This function has not been completed yet. 
  
  Created: 12th May 2023
  Last Update: 22th May 2023
  Created By: Michael Haggart 
  For: StarthAIS
  Updated by: Michael Haggart 
              #Add New Names Here
*/

#ifndef AISFCbarometer
#define AISFCbarometer

#include <Adafruit_BMP085.h>
#include <math.h>

//Adafruit_BMP085 bmp;

class AISFCbaro : public Adafruit_BMP085 {
public:
  float calibrateBMP(bool& cs);
  float zeroAlt();
  float curAlt(float zAlt);
  float mtoFeet(float cAlt);
  float getPressure();
  void readoutPressure();
  void readoutBMP();
  bool AscendingDecendingCheck(float cAlt, float pAlt);
};

float AISFCbaro::calibrateBMP(bool& cs) {
  float calibratedPressure{}, pressureSum{};
  for (int i = 0; i < 50; i++) {
    delay(10);
    pressureSum = +this->readPressure();
    if (i != 49) {
      continue;
    }
    if (i == 49) {
      calibratedPressure = pressureSum / 50;
      cs = true;
      return calibratedPressure;
    } else {
      cs = false;
      continue;
    }
  }
}

float AISFCbaro::zeroAlt() {
  float totalAlt{};
  for (int i = 0; i < 50; i++) {
    totalAlt =+ this->readAltitude();
  }
  float zAlt = totalAlt/50;
  return zAlt;
}

float AISFCbaro::curAlt(float zAlt)
{
  float alt = this->readAltitude() - zAlt;
  return alt;
}

float AISFCbaro::mtoFeet(float cAlt)
{
  float cAltFeet = cAlt / 0.3048;
  return cAltFeet;
}

float AISFCbaro::getPressure() {
  return this->readPressure();
}

void AISFCbaro::readoutPressure() {
  Serial.print("Pressure: ");
  Serial.print(this->readPressure());
  Serial.println(" Pa");
}

void AISFCbaro::readoutBMP() {
  Serial.print("Temperature = ");
  Serial.print(this->readTemperature());
  Serial.println(" *C");
  Serial.print("Pressure = ");
  Serial.print(this->readPressure());
  Serial.println(" Pa");
  // Calculate altitude assuming 'standard' barometric
  // pressure of 1013.25 millibar = 101325 Pascal
  Serial.print("Altitude = ");
  Serial.print(this->readAltitude());
  Serial.println(" meters");
  Serial.print("Pressure at sealevel (calculated) = ");
  Serial.print(this->readSealevelPressure());
  Serial.println(" Pa");
  // you can get a more precise measurement of altitude
  // if you know the current sea level pressure which will
  // vary with weather and such. If it is 1015 millibars
  // that is equal to 101500 Pascals.
  Serial.print("Real altitude = ");
  Serial.print(this->readAltitude(101500));
  Serial.println(" meters");
  Serial.println();
}

bool AscendingDecendingCheck(float cAlt, float pAlt)
{
  bool Ascending = true; //if true, current alt is greater than previous alt, if false, inverse.
  if(cAlt > pAlt)
  {
    Ascending = false; 
    return Ascending;
  }
  else
  {
    return Ascending;
  }
}

#endif