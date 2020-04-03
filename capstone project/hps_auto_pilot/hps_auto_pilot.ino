#include <Arduino_LSM9DS1.h>
#include <Arduino_LPS22HB.h>
#include <Arduino_HTS221.h>
#include <Arduino_APDS9960.h>
//#include <AudioSound.h>
//#include <PDM.h>

#include <stdio.h>
unsigned long time1;

void setup() {
  // put your setup code here, to run once:
  //initialization 
  Serial.begin(9600);
  if (!IMU.begin()) {
      Serial.println("Failed to initialize IMU!");
      while (1);
  }

  if (!APDS.begin()) {
    Serial.println("Error initializing APDS9960 sensor!");
  }

  if (!HTS.begin()) {
    Serial.println("Failed to initialize humidity temperature sensor!");
    while (1);
  }
  // for setGestureSensitivity(..) a value between 1 and 100 is required.
  // Higher values makes the gesture recognition more sensible but less accurate
  // (a wrong gesture may be detected). Lower values makes the gesture recognition
  // more accurate but less sensible (some gestures may be missed).
  // Default is 80
  //APDS.setGestureSensitivity(80);

  //Serial.println("Detecting gestures ...");
}

void loop() {

  // put your main code here, to run repeatedly:
  delay(1000);
  time1 = millis();
  Serial.println(time1);
  
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
      Serial.print(F("x: "));\
      Serial.print(mag_x);
      Serial.print(" \t");
      Serial.print(F("y: "));
      Serial.print(mag_y);
      Serial.print(" \t");
      Serial.print(F("z: "));
      Serial.print(mag_z);
      Serial.println("uT");
  }
/*
  //Gesture sensor
  if (APDS.gestureAvailable()) {
    // a gesture was detected, read and print to serial monitor
    int gesture = APDS.readGesture();

    switch (gesture) {
      case GESTURE_UP:
        Serial.println("Detected UP gesture");
        break;

      case GESTURE_DOWN:
        Serial.println("Detected DOWN gesture");
        break;

      case GESTURE_LEFT:
        Serial.println("Detected LEFT gesture");
        break;

      case GESTURE_RIGHT:
        Serial.println("Detected RIGHT gesture");
        break;

      default:
        // ignore
        break;
    }
  }
*/
  //color sensor
  while (! APDS.colorAvailable()) {
    delay(5);
  }
  int r, g, b;

  // read the color
  APDS.readColor(r, g, b);
  // print the values
  Serial.print("r = ");
  Serial.println(r);
  Serial.print("g = ");
  Serial.println(g);
  Serial.print("b = ");
  Serial.println(b);
/*
  //proximity
  if (APDS.proximityAvailable()) {
    // read the proximity
    // - 0   => close
    // - 255 => far
    // - -1  => error
    int proximity = APDS.readProximity();
    // print value to the Serial Monitor
    Serial.println(proximity);
  }
  */

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
  /*
  //Pulse Density Modulation
  // buffer to read samples into, each sample is 16-bits
  short sampleBuffer[256];
  
  // number of samples read
  volatile int samplesRead;
  
  //…

  // query the number of bytes available
  int bytesAvailable = PDM.available();

  // read into the sample buffer
  int bytesRead = PDM.read(sampleBuffer, bytesAvailable);

  // 16-bit, 2 bytes per sample
  samplesRead = bytesRead / 2;
  Serial.println(sampleBuffer);
*/

}
