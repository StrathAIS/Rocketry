/*
  Barometer control header

  This header is to provide a set of functions for use with the Barometer.
	**This is Mission Critical**
  The barometer will control deployment of shutes. This function has not been completed yet.

  Created: 22th May 2023
  Last Update: 22th May 2023
  Created By: Michael Haggart
  For: StarthAIS
  Updated by: Michael Haggart
			  #Add New Names Here
*/
//
//
#ifndef AISFCbarometer
#define AISFCbarometer
//
//
#include <Adafruit_BMP085.h>
#include <math.h>
//
//
class AISFCbaro : public Adafruit_BMP085
{
public:
	float getAltPa() { return this->readPressure(); }
	float zeroAlt();
	float curAlt(float zAlt);
	float highestAlt{};
	float m_to_feet(float cAlt);
};
//
//
float AISFCbaro::zeroAlt() 
{
	float totalAlt{};
	for (int i = 0; i < 50; i++) 
	{
		totalAlt = +this->readAltitude();
	}
	float zAlt = totalAlt / 50;
	return zAlt;
}
//
//
float AISFCbaro::curAlt(float zAlt)
{
	float curAlt = this->readAltitude() - zAlt;
	return curAlt;
}
//
//
float AISFCbaro::m_to_feet(float cAlt)
{
	float cAltFeet = cAlt / 0.3048;
	return cAltFeet;
}
//
//
#endif //!AISFCbarometer