/*
  Data Logging Header

  This header file defines a functions we may wish to use for data logging purposes.
  Currently the SD and File classes are used unmodified in the Main program.
  For future, we may wish to expand the functionality of these classes.
  This is the purpose of this code. When telecommunications are in place, functions form here
  will also be used in that header for transmitting packaged data.

  Created: 24th May 2023
  Last Update: 24th May 2023
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
#include <math.h>

char nmeaBuffer[100];
MicroNMEA nmea(nmeaBuffer, sizeof(nmeaBuffer));
constexpr auto Radius = 6371000;
constexpr auto pi = 3.14159265358979;
//
//
bool initGPS(SFE_UBLOX_GNSS AISFCgps);
bool updateGPS(SFE_UBLOX_GNSS AISFCgps, long& lat_mdeg, long& long_mdeg, long& gnss_alt);
void zeroLaunchSite(SFE_UBLOX_GNSS AISFCgps, long& launch_Lat, long& launch_Long);
//
//
bool initGPS(SFE_UBLOX_GNSS AISFCgps)
{
	if (!AISFCgps.begin())
	{
		Serial.println(F("u-blox GNSS not detected at default I2C address. Please check wiring. Freezing."));
		while (1);
		return false;
	}
	AISFCgps.setI2COutput(COM_TYPE_UBX | COM_TYPE_NMEA); //Set the I2C port to output both NMEA and UBX messages
	AISFCgps.saveConfigSelective(VAL_CFG_SUBSEC_IOPORT); //Save (only) the communications port settings to flash and BBR
	AISFCgps.setProcessNMEAMask(SFE_UBLOX_FILTER_NMEA_ALL); // Make sure the library is passing all NMEA messages to processNMEA
	AISFCgps.setProcessNMEAMask(SFE_UBLOX_FILTER_NMEA_GGA); // Or, we can be kind to MicroNMEA and _only_ pass the GGA messages to it
	return true;
}
//
//
bool updateGPS(SFE_UBLOX_GNSS AISFCgps, long& lat_mdeg, long& long_mdeg, long& gnss_alt)
{
	if (AISFCgps.checkUblox())
	{
		if (nmea.isValid())
		{
			lat_mdeg = nmea.getLatitude();
			long_mdeg = nmea.getLongitude();
			gnss_alt = nmea.getAlt_long();  // <- WARNING this function is custom added by Mike 24th May 2023
			nmea.clear();
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		Serial.println("Waiting for fresh GPS data");
		return false;
	}
}
//
//
void zeroLaunchSite(SFE_UBLOX_GNSS AISFCgps, long& launch_Lat, long& launch_Long)
{
	for (int i = 0; i < 10; i++)
	{
		launch_Lat = +nmea.getLatitude();
		launch_Long = +nmea.getLongitude();
	}
	launch_Lat = launch_Lat / 10;
	launch_Long = launch_Long / 10;
}
//
//
void distance_bearing(long launch_Lat, long launch_Long, long current_Lat, long current_Long)
{
	/*
const R = 6371e3; // metres
const φ1 = lat1 * Math.PI/180; // φ, λ in radians
const φ2 = lat2 * Math.PI/180;
const Δφ = (lat2-lat1) * Math.PI/180;
const Δλ = (lon2-lon1) * Math.PI/180;
const a = Math.sin(Δφ/2) * Math.sin(Δφ/2) +
		  Math.cos(φ1) * Math.cos(φ2) *
		  Math.sin(Δλ/2) * Math.sin(Δλ/2);
const c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1-a));
const d = R * c; // in metres
	*/
	/*HAVERSINE FORMULA*/
	float deltaLat = ((float)launch_Lat - (float)current_Lat) * (pi / 180);
	float deltaLong = ((float)current_Long - (float)launch_Long) * (pi / 180);
	auto a = // <- a is the square of half the chord length between the points
		sin(deltaLat / 2) * sin(deltaLat / 2) +
		cos(((float)launch_Lat * (pi / 180))) * cos(((float)current_Lat * (pi / 180))) *
		sin(deltaLong / 2) * sin(deltaLong / 2);
	auto c = // <- c is the angular distance in radians
		2 * atan2(sqrt(a), sqrt(1 - a));
	auto d =
		Radius * c;
	/*
 Formula:	θ = atan2( sin Δλ ⋅ cos φ2 , cos φ1 ⋅ sin φ2 − sin φ1 ⋅ cos φ2 ⋅ cos Δλ )
 where	φ1,λ1 is the start point, φ2,λ2 the end point (Δλ is the difference in longitude)
 const y = Math.sin(λ2 - λ1) * Math.cos(φ2);
 const x = Math.cos(φ1) * Math.sin(φ2) -
	 Math.sin(φ1) * Math.cos(φ2) * Math.cos(λ2 - λ1);
 const θ = Math.atan2(y, x);
 const brng = (θ * 180 / Math.PI + 360) % 360; // in degrees
	 */
	 /*BEARING*/
	 /*I DO NOT TRUST THIS, TEST AND CONFIRM*/
	auto y =
		sin((float)current_Long - (float)launch_Long) * cos((float)current_Lat * (pi / 180));
	auto x =
		cos((float)launch_Lat * (pi / 180)) * sin((float)current_Lat * (pi / 180)) -
		sin((float)launch_Lat * (pi / 180)) * cos((float)current_Lat * (pi / 180)) * cos((float)current_Long - (float)launch_Long);
	auto theta = atan2(y, x);
	auto brng = theta * 180 / pi + 360 % 360;
}
//
//
void SFE_UBLOX_GNSS::processNMEA(char incoming)
{
	//Take the incoming char from the u-blox I2C port and pass it on to the MicroNMEA lib
	//for sentence cracking
	nmea.process(incoming);
}
#endif
