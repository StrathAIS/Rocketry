/*
  Data Logging Header

  This header file defines a functions we may wish to use for data logging purposes.
  Currently the SD and File classes are used unmodified in the Main program.
  For future, we may wish to expand the functionality of these classes.
  This is the purpose of this code. When telecommunications are in place, functions form here
  will also be used in that header for transmitting packaged data.

  Created: 23th May 2023
  Last Update: 23th May 2023
  Created By: Michael Haggart
  For: StarthAIS
  Updated by: Michael Haggart
              #Add New Names Here
*/
#ifndef AISFCDataLogging
#define AISFCDataLogging
//
//
#include <SPI.h>
#include <SD.h>
#include <string.h>
//
//
File dataLogInit(File& log);
String loggedData(float millisecs, float pressure_bmp, float alt, float x_accel, float y_accel, float z_accel, long latGPS, long longGPS);
void writeEntry(String entry, File& log);
//
//
File dataLogInit(File& log) 
{
    log = SD.open("Log.txt", FILE_WRITE);
    if (log) 
    {
        Serial.print("Writing to Log.txt");
        log.println("milliseconds,pressure,x_accel,y_accel,x_accel");
        log.close();
        Serial.println("Log written");
    }
    else 
    {
        Serial.println("Error opening Log.txt");
    }
    log = SD.open("Log.txt");
    if (log) 
    {
        Serial.println("Log.txt contents");
        while (log.available()) 
        {
            Serial.write(log.read());
        }
        log.close();
    }
    else {
        Serial.println("Error opening Log.txt");
    }
}
//
//
String loggedData(float millisecs, float pressure_bmp, float alt, float x_accel, float y_accel, float z_accel, long latGPS, long longGPS)
{
    String data = String(millisecs) + "," + String(pressure_bmp) + "," + String(x_accel) + "," + String(y_accel) + "," + String(x_accel) + "," + String(latGPS) + "," + String(longGPS) + '\n';
    return data;
}
//
//
void writeEntry(String entry, File& log)
{
    log = SD.open("Log.txt", FILE_WRITE);
    if (log) 
    {
        Serial.print("Writing to Log.txt");
        log.println(entry);
        log.close();
        Serial.println("Log written");
    }
    else 
    {
        Serial.println("Error opening Log.txt");
    }
}
//
//
#endif
