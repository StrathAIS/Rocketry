#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

float accel_sum,accel_offset;
int readings = 0;



float get_accel();


void setup(void) {
  Serial.begin(115200);

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  // set accelerometer range to +-8G
  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);

  // set gyro range to +- 500 deg/s
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);

  // set filter bandwidth to 21 Hz
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);


// Calculate Offset 
for (int i = 0; i <100;i++)  // Take 10 readings then average
  {
  
      //accel_x_sum = accel_x_sum + accel_x; // BAROMETER DATA
      accel_sum = accel_sum + get_accel();                  // ACCELEROMETER DATA

     delay(10);

  }
    accel_offset = accel_sum/100;
    accel_sum=0;

    Serial.print("accel_avg = ");
    Serial.print(accel_offset);



  

  delay(100);
}



void loop() {
  /* Get new sensor events with the readings */


get_accel();

 
//  sensors_event_t a, g, temp;
//  mpu.getEvent(&a, &g, &temp);

  /* Print out the values */
  //Serial.print("Acceleration X: ");
//  Serial.print(readings);
//  Serial.print(",");
//  Serial.print(a.acceleration.x);
//  Serial.print(",");
//  Serial.print(a.acceleration.y);
//  Serial.print(",");
//  Serial.println(a.acceleration.z);
//  Serial.println(" m/s^2");
//
//  Serial.print("Rotation X: ");
//  Serial.print(g.gyro.x);
//  Serial.print(", Y: ");
//  Serial.print(g.gyro.y);
//  Serial.print(", Z: ");
//  Serial.print(g.gyro.z);
//  Serial.println(" rad/s");
//
//  Serial.print("Temperature: ");
//  Serial.print(temp.temperature);
//  Serial.println(" degC");
  

  Serial.print(readings);
  Serial.print(",");
  Serial.println(get_accel()); 
  delay(100);

  readings++;
  
}




float get_accel() // Assumes -ve x axis is straight upwards according to PCB
{
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  return a.acceleration.x - accel_offset;

}
