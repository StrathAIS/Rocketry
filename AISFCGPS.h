/*
  GPS control header 

  This header is to provide a set of functions for use with the GPS.
  As of 19th May 2023 we only look at the location given by the GPS. 
  We may wish to use the time and date function for logging purposes. 
  We should also intend to create a function that provides heading and distance from a known point.
  This is to aid in recovery of the rocket after it launches.
  
  Created: 12th May 2023
  Last Update: 19th May 2023
  Created By: Michael Haggart 
  For: StarthAIS
  Updated by: Michael Haggart 
              #Add New Names Here
*/

#ifndef AISFCGPS
#define AISFCGPS

#include <SparkFun_u-blox_GNSS_Arduino_Library.h>
#include <u-blox_config_keys.h>
#include <u-blox_structs.h>
#include <MicroNMEA.h>

//SFE_UBLOX_GNSS NEO_M9;
char nmeaBuffer[100];
MicroNMEA nmea(nmeaBuffer, sizeof(nmeaBuffer));

void startGPS(SFE_UBLOX_GNSS gps);

void startGPS(SFE_UBLOX_GNSS gps)
{
	Wire.begin();
	if (gps.begin() == false)
	{
		Serial.println(F("u-blox GNSS not detected at default I2C address. Please check wiring. Freezing."));
		while (1);
	}
	gps.setI2COutput(COM_TYPE_UBX | COM_TYPE_NMEA); //Set the I2C port to output both NMEA and UBX messages
	gps.saveConfigSelective(VAL_CFG_SUBSEC_IOPORT); //Save (only) the communications port settings to flash and BBR
	gps.setProcessNMEAMask(SFE_UBLOX_FILTER_NMEA_ALL); // Make sure the library is passing all NMEA messages to processNMEA
	gps.setProcessNMEAMask(SFE_UBLOX_FILTER_NMEA_GGA); // Or, we can be kind to MicroNMEA and _only_ pass the GGA messages to it
}



#endif