/*
  Accelerometer control header

  This header is to control the accelerometer functions of the flight computer.
    **This is Mission Critical**
  The impulse of launch is used by the accelerometer to tell the Flight Computer to
  allow the barometer to control the charges.
    **Basically: if (Launched == true){
                    bool depChargeArm = true; }

  Created: 22th May 2023
  Last Update: 22th May 2023
  Created By: Michael Haggart
  For: StarthAIS
  Updated by: Michael Haggart
              #Add New Names Here
*/

#ifndef AISFCAccelerometer
#define AISFCAccelerometer

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>  //Needed for I2C to GY-521

void get_xya(const int address, float& x_accel, float& y_accel, float& z_accel);
float absoluteAcceleration(float x_accel, float y_accel, float z_accel);
//
//
void get_xya(const int address, float& x_accel, float& y_accel, float& z_accel)
{
    Wire.beginTransmission(address);
    Wire.write(0x3B);                         // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
    Wire.endTransmission(false);              // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
    Wire.requestFrom(address, 7 * 2, true);  // request a total of 7*2=14 registers
    x_accel = Wire.read() << 8 | Wire.read();
    y_accel = Wire.read() << 8 | Wire.read();
    z_accel = Wire.read() << 8 | Wire.read();
    x_accel = ((float)x_accel / 2048) * 9.806;;      //<- -/+16g = 2048 LSB/g * 9.806 for m/s^2
    y_accel = ((float)y_accel / 2048) * 9.806;;      //<- -/+16g = 2048 LSB/g * 9.806 for m/s^2
    z_accel = ((float)z_accel / 2048) * 9.806;;      //<- -/+16g = 2048 LSB/g * 9.806 for m/s^2 
}
//
//
float absoluteAcceleration(float x_accel, float y_accel, float z_accel)
{
    float x_accel_g = (x_accel / 2048) * 9.806;      //<- -/+16g = 2048 LSB/g * 9.806 for m/s^2
    float y_accel_g = (y_accel / 2048) * 9.806;      //<- -/+16g = 2048 LSB/g * 9.806 for m/s^2
    float z_accel_g = (z_accel / 2048) * 9.806;      //<- -/+16g = 2048 LSB/g * 9.806 for m/s^2
    float absoluteAccel = sqrt((x_accel_g * x_accel_g) + (y_accel_g * y_accel_g) + (z_accel_g * z_accel_g));
    return absoluteAccel;
}
//
//
#endif //!AISFCAccelerometer