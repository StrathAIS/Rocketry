/*
  Main sketch file 

  This program is for the StrathAIS Flight Computer (AISFC)
  It combines multiple header files together and dictates the functionality used 
    **This program is build around the Arduino Mega dev board** 
    **Please bear this in mind when using this code for other boards**
    **Pin outs may be different**
  
  Created: May 2023
  Last Update: 22th May 2023
  Created By: Michael Haggart 
  For: StarthAIS
  Updated by: Michael Haggart 
              #Add New Names Here
*/

#include "AISFCGPS.h"
#include "AISFCAccelerometer.h"
#include "AISFCBarometer.h"
#include "AISFCLED.h"
#include "AISFCTelecoms.h"
#include "AISFCDataLogging.h"

enum flightStatus { preLaunch,
                    Boost,
                    Coast,
                    Apogee,
                    Drogue,
                    Main,
                    Landed };

Adafruit_MPU6050 mpu1;
Adafruit_Sensor *mpu1_temp, *mpu1_accel, *mpu1_gyro;
float x_accel1{}, y_accel1{}, z_accel1{};
float timeSinceActivate{};
float baroPressure{}, zAlt{}, baroAlt{}, baroAltFeet{};
const int mpu1_address = 0x69;
int32_t latGPS{}, longGPS{};
int sampleCountAscCheck{};
float apogeeAlt{}; 
float absoluteAccel{};
#define drogueIgPin 3
#define mainIgPin 4
flightStatus activeFlightStatus = 0;

SFE_UBLOX_GNSS NEO_M9;

AISFCbaro baro;
File dataLog;

bool activateHardware();
int stateCheckFunc(flightStatus& fS, bool apogeeCheck, bool motorCheck, float curAlt) 

void ignitionControl(bool apogeeCheck, uint8_t pin, flightStatus fS);
bool descendingCheck(int& sampleCount, float& apogeeAlt, float cAlt);
bool apogeeCheck = false;
bool motorCheck = false;
//bool safetyContol(float time, bool apogeeCheck); <- to be expanded

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  digitalWrite(drogueIgPin, LOW);  // Set Drogue activation pin to Low
  digitalWrite(mainIgPin, LOW);    // Set Main activation pin to High

  Serial.println("Test bed for Arduino Flight Computer");
  if (!activateHardware()) {
    Serial.println("Not all hardware activated successfully, check terminal log for failed device");
    while (1) {
      delay(10);
    }
  }
  if (activateHardware()) {
    Serial.println("Hardware activated successfully");
  }
  mpu1.getAccelerometerSensor();
  // mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
  // mpu.setMotionDetectionThreshold(1);
  // mpu.setMotionDetectionDuration(20);
  // mpu.setInterruptPinLatch(true);	// Keep it latched.  Will turn off when reinitialized.
  // mpu.setInterruptPinPolarity(true);
  // mpu.setMotionInterrupt(true);
  AISFCAccelerometer::getSensors(mpu1, mpu1_accel, mpu1_gyro, mpu1_temp);
  AISFCDataLogging::dataLogInit(dataLog);
  zAlt = baro.zeroAlt();
}

void loop() {
  // put your main code here, to run repeatedly:
  AISFCAccelerometer::printSensors(mpu1_address);
  AISFCAccelerometer::get_xya(mpu1_address, x_accel1, y_accel1, z_accel1);
  absoluteAccel = AISFCAccelerometer::absoluteAcceleration();
  motorCheck = AISFCAccelerometer::motorCheckFunction(absoluteAccel);
  baroPressure = baro.getPressure();
  baroAlt = baro.curAlt(zAlt);
  if (apogeeCheck == false) {
    apogeeCheck = descendingCheck(sampleCountAscCheck, apogeeAlt, baroAlt);
  }
  
  Serial.print("Highest Alt: ");
  Serial.println(apogeeAlt);
  baroAltFeet = baro.mtoFeet(baroAlt);
  Serial.print("Current Alt in Feed: ");
  Serial.println(baroAltFeet);

  latGPS = NEO_M9.getLatitude();
  longGPS = NEO_M9.getLongitude();
  timeSinceActivate = millis();

  //int stateCheckFunc(flightStatus& fS, bool apogeeCheck, bool motorCheck, float curAlt) 

  //milliseconds, pascals, meters, G's?, G's?, G's?, degrees, degrees
  String entry = AISFCDataLogging::loggedData(timeSinceActivate, baroPressure, baroAlt, x_accel1, y_accel1, z_accel1, longGPS, latGPS);
  AISFCDataLogging::writeEntry(entry, dataLog);
}

bool activateHardware() {
  bool accelFlag01{}, SDFlag{}, gpsFlag{}, baroFlag{};
  if (!mpu1.begin(mpu1_address)) {
    Serial.println("Accelerometer 1 failed to activate");
    accelFlag01 = false;
  }
  if (mpu1.begin(mpu1_address)) {
    Serial.println("Accelerometer 1 activated");
    accelFlag01 = true;
  }
  if (!SD.begin(4)) {
    Serial.println("SD data logger failed to activate");
    SDFlag = false;
  }
  if (SD.begin(4)) {
    Serial.println("SD data logger activated");
    if (dataLog) {
      dataLog.println("Flight Log");
    }
    SDFlag = true;
  }
  if (!NEO_M9.begin()) {
    Serial.println("GPS failed to activate");
    gpsFlag = false;
  }
  if (NEO_M9.begin()) {
    Serial.println("GPS activated");
    startGPS(NEO_M9);
    gpsFlag = true;
  }
  if (!baro.begin()) {
    Serial.println("Barometer failed to activate");
    baroFlag = false;
  }
  if (baro.begin()) {
    Serial.println("Barometer activated");
    baroFlag = true;
  }

  if (accelFlag01 == true && SDFlag == true) {
    return true;
  } else {
    return false;
  }
}

int stateCheckFunc(flightStatus& fS, bool apogeeCheck, bool motorCheck, float curAlt) 
{
  //The motorCheck is bool derived from the current acceleration of the rocket. If it is above some threshhold, then the motor is currently firing
  //If this is true, then the rocket is in its boost phase
  //If this is false && apogeeCheck is false, the rocket is in its Coast phase. 
  if(apogeeCheck == false && motorCheck == false && curAlt <= 50)
  {
    fS = preLaunch;
    return 0;
  }
  if(apogeeCheck == false && motorCheck == true && curAlt >= 10)
  {
    fS = Boost; 
    return 1;
  }
  if(apogeeCheck == false && motorCheck == false && curAlt >= 100)
  {
    fS = Coast;
    return 2;
  }
  if(apogeeCheck == true && motorCheck == false && curAlt >= 2000)
  {
    fS = Apogee;
    return 3;
  }
  //if(apogeeCheck )
}

void ignitionControl(bool apogeeCheck, uint8_t pin, flightStatus fS) {
  switch (fS) {
    case 0:       //<- preLaunch

      break;
    case 1:       //Boost

      break;
    case 2:       //Coast

      break;
    case 3:       //Apogee
    //the change from coast to apogee to drogue should be very short as the drogue should deploy once the apogee is confirmed
      break;
    case 4:       //Drogue
      digitalWrite(drogueIgPin, HIGH);  // Fire the Drogue
      break;
    case 5:       //Main
      digitalWrite(mainIgPin, HIGH);    // Fire the Main
      break;
    case 6:       //Landed

      break;
  }
}

bool descendingCheck(int& sampleCount, float& apogeeAlt, float cAlt) {
  if ((apogeeAlt - cAlt) >= 1)  //if current alt is lower than 1 meter below apogee
  {
    sampleCount = sampleCount + 1;  //counter increments
    apogeeAlt = cAlt;               //apogee = current alt
    if (sampleCount == 15)          //if 15 counts are sucessful return true;
    {
      return true;
    }
    return false;
  }
  if ((apogeeAlt - cAlt) < 1)  //if current alt is greater than 1 meter above apogee
  {
    apogeeAlt = apogeeAlt;  //apogee
    sampleCount = 0;
    return false;
  }
}


/*bool safetyContol(float time, bool apogeeCheck)
{

}*/