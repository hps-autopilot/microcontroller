#include <Arduino_LSM9DS1.h>  //IMU
#include <Arduino_LPS22HB.h>  //Pressure Sensor
#include <Arduino_HTS221.h>   //Humidity & Temperature Sensor

#include <stdio.h>
unsigned long timer;

void setup() {
  // put your setup code here, to run once:
  //initialization 
  Serial.begin(9600);
  if (!IMU.begin()) {
      Serial.println("Failed to initialize IMU!");
      while (1);
  }
  if (!BARO.begin()) {
    Serial.println("Failed to initialize pressure sensor!");
    while (1);
  }

  if (!HTS.begin()) {
    Serial.println("Failed to initialize humidity temperature sensor!");
    while (1);
  }
 
}

void loop() {

  // put your main code here, to run repeatedly:
  delay(1000);
  timer = millis();
  Serial.println(timer);
  
  //pressure sensor
  float pressure = BARO.readPressure();
  Serial.print("Pressure = ");
  Serial.print(pressure);
  Serial.println(" kPa");

  //IMU
  float x, y, z;
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);
    //x is the long side angle , y is the short side angle, z is surface angle
    Serial.print(F("x: "));
    Serial.print(int(x*100)/100.0);
    Serial.print(" \t");
    Serial.print(F("y: "));
    Serial.print(int(y*100)/100.0);
    Serial.print(" \t");
    Serial.print(F("z: "));
    Serial.println(int(z*100)/100.0);  
  }

  float yaw, pitch, roll;

  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(roll, pitch, yaw);

    Serial.print(F("roll: "));
    Serial.print(roll);
    Serial.print(" \t");
    Serial.print(F("pitch: "));
    Serial.print(pitch);
    Serial.print(" \t");
    Serial.print(F("yaw: "));
    Serial.println(yaw);
  }
  
  float mag_x, mag_y, mag_z;

  if (IMU.magneticFieldAvailable()) {

      IMU.readMagneticField(mag_x, mag_y, mag_z);
      Serial.print(F("mag_x: "));\
      Serial.print(mag_x);
      Serial.print(" \t");
      Serial.print(F("mag_y: "));
      Serial.print(mag_y);
      Serial.print(" \t");
      Serial.print(F("mag_z: "));
      Serial.print(mag_z);
      Serial.println("uT");
  }

  //temperature
  float temperature = HTS.readTemperature();
  Serial.print("Temperature = ");
  Serial.print(temperature);
  Serial.println(" °C");

  //humidity
  float humidity    = HTS.readHumidity();
  Serial.print("Humidity    = ");
  Serial.print(humidity);
  Serial.println(" %");
  Serial.println();


}
