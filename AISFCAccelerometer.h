/*
  Accelerometer control header 

  This header is to control the accelerometer functions of the flight computer.
    **This is Mission Critical**
  The impulse of launch is used by the accelerometer to tell the Flight Computer to 
  allow the barometer to control the charges. 
    **Basically: if (Launched == true){
                    bool depChargeArm = true; }
  
  Created: 12th May 2023
  Last Update: 20th May 2023
  Created By: Michael Haggart 
  For: StarthAIS
  Updated by: Michael Haggart 
              #Add New Names Here
*/

#ifndef AISFCAccelerometer
#define AISFCAccelerometer

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>                                  //Needed for I2C to GY-521

int16_t accelerometer_x, accelerometer_y, accelerometer_z;  // variables for accelerometer raw data
int16_t gyro_x, gyro_y, gyro_z;                             // variables for gyro raw data
int16_t temperature;                                        // variables for temperature data
sensors_event_t ac, gy, te;


char temp_str[7];                                              // temporary variable used in convert function

char* convert_int16_to_str(int16_t i);   // converts int16 to string. Moreover, resulting strings will have the same length in the debug monitor.
void getSensors(Adafruit_MPU6050 mpu, Adafruit_Sensor* accelSens, Adafruit_Sensor* gyroSens, Adafruit_Sensor* tempSens);
void printSensors(Adafruit_MPU6050 mpu);
void setRange(Adafruit_MPU6050 mpu);
void get_xya(const int address, float& x_accel,float& y_accel,float& z_accel);
float absoluteAcceleration();
bool motorCheckFunction(float absoluteAccel);
//int setFilLScaleAccelRange(MPU6050_ACCEL_FS_16);



char* convert_int16_to_str(int16_t i)   // converts int16 to string. Moreover, resulting strings will have the same length in the debug monitor.
{
	sprintf(temp_str, "%6d", i);           //"string print format" (to char array temp_str, signed decimal thats 6 chars long, from i)
	return temp_str;
}

void getSensors(Adafruit_MPU6050 mpu, Adafruit_Sensor* accelSens, Adafruit_Sensor* gyroSens, Adafruit_Sensor* tempSens)
{
	accelSens = mpu.getAccelerometerSensor();
	accelSens->printSensorDetails();
	gyroSens = mpu.getGyroSensor();
	gyroSens->printSensorDetails();
	tempSens = mpu.getTemperatureSensor();
	tempSens->printSensorDetails();
}

void setRange(Adafruit_MPU6050 mpu)
{
	mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
	mpu.setGyroRange(MPU6050_RANGE_250_DEG);
	mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

void printSensors(const int address)
{
	Wire.beginTransmission(address);
	Wire.write(0x3B);                         // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
	Wire.endTransmission(false);              // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
	Wire.requestFrom(address, 7 * 2, true);  // request a total of 7*2=14 registers
	accelerometer_x = Wire.read() << 8 | Wire.read();  // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L)
	accelerometer_y = Wire.read() << 8 | Wire.read();  // reading registers: 0x3D (ACCEL_YOUT_H) and 0x3E (ACCEL_YOUT_L)
	accelerometer_z = Wire.read() << 8 | Wire.read();  // reading registers: 0x3F (ACCEL_ZOUT_H) and 0x40 (ACCEL_ZOUT_L)
	temperature = Wire.read() << 8 | Wire.read();      // reading registers: 0x41 (TEMP_OUT_H) and 0x42 (TEMP_OUT_L)
	gyro_x = Wire.read() << 8 | Wire.read();           // reading registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
	gyro_y = Wire.read() << 8 | Wire.read();           // reading registers: 0x45 (GYRO_YOUT_H) and 0x46 (GYRO_YOUT_L)
	gyro_z = Wire.read() << 8 | Wire.read();           // reading registers: 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L)
	Serial.print("aX = ");
	Serial.println(convert_int16_to_str(accelerometer_x));
	Serial.println(ac.acceleration.x);
	Serial.print(" | aY = ");
	Serial.println(convert_int16_to_str(accelerometer_y));
	Serial.println(ac.acceleration.y);
	Serial.print(" | aZ = ");
	Serial.println(convert_int16_to_str(accelerometer_z));
	Serial.println(ac.acceleration.z);
}

void get_xya(const int address, float& x_accel,float& y_accel,float& z_accel)
{
  Wire.beginTransmission(address);
	Wire.write(0x3B);                         // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
	Wire.endTransmission(false);              // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
	Wire.requestFrom(address, 7 * 2, true);  // request a total of 7*2=14 registers
  x_accel = Wire.read() << 8 | Wire.read(); 
  y_accel = Wire.read() << 8 | Wire.read(); 
  z_accel = Wire.read() << 8 | Wire.read(); 
  x_accel = ((float)x_accel / 2048)*9.806;;      //<- -/+16g = 2048 LSB/g * 9.806 for m/s^2
  y_accel = ((float)y_accel / 2048)*9.806;;      //<- -/+16g = 2048 LSB/g * 9.806 for m/s^2
  z_accel = ((float)z_accel / 2048)*9.806;;      //<- -/+16g = 2048 LSB/g * 9.806 for m/s^2 
}

float absoluteAcceleration()
{
  /*
    I'm not sure how best to go about this. 
    I think it's important to keep the direction of acceleration in mind to avoid 
    any false possitives/negatives during changes of acceleration.
    The concern is that I don't fully know what units the GY521 is giving. 
    This any maths I perform could give an incorrect result. If I just go for sqrt(x^2+y^2+z^2)
    then I worry that will make the FC blind to its actual current flight condition (falling, rising etc).
  */
  Wire.beginTransmission(0x69);
	Wire.write(0x3B);                         // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
	Wire.endTransmission(false);              // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
	Wire.requestFrom(0x69, 7 * 2, true);  // request a total of 7*2=14 registers
  float x_accel = Wire.read() << 8 | Wire.read(); 
  float y_accel = Wire.read() << 8 | Wire.read(); 
  float z_accel = Wire.read() << 8 | Wire.read(); 

  float x_accel_g = ((float)x_accel / 2048)*9.806;      //<- -/+16g = 2048 LSB/g * 9.806 for m/s^2
  float y_accel_g = ((float)y_accel / 2048)*9.806;      //<- -/+16g = 2048 LSB/g * 9.806 for m/s^2
  float z_accel_g = ((float)z_accel / 2048)*9.806;      //<- -/+16g = 2048 LSB/g * 9.806 for m/s^2

  float absoluteAccel = sqrt((x_accel_g*x_accel_g) + (y_accel_g*y_accel_g) + (z_accel_g*z_accel_g));
  return absoluteAccel;
}

bool motorCheckFunction(float absoluteAccel)
{
  bool motorCheck = false;
  if(absoluteAccel > 15)
  {
    return motorCheck = true;
  }
  else
  {
    return motorCheck = false;
  }
}

#endif

