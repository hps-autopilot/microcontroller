#include <Arduino_LSM9DS1.h>  //IMU
#include <Arduino_LPS22HB.h>  //Pressure Sensor
#include <Arduino_HTS221.h>   //Humidity & Temperature Sensor
#include <Arduino_APDS9960.h> //Gesture, Light, Proximity Sensors

#include <stdio.h>
unsigned long timer;
int delay_freq = 1000;

void setup() {
  // put your setup code here, to run once:
  //initialization
  Serial.println(1);
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

  if (!APDS.begin()) {
    Serial.println("Error initializing APDS9960 sensor.");
  }
}

void loop() {

  // put your main code here, to run repeatedly:

  //time stamp
  delay(delay_freq);
  timer = millis();
  Serial.print("Time: ");
  Serial.print(timer / 60000);
  Serial.print(":");
  Serial.println((timer % 60000) / 1000);

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
    Serial.print(int(x * 100) / 100.0);
    Serial.print(" \t");
    Serial.print(F("y: "));
    Serial.print(int(y * 100) / 100.0);
    Serial.print(" \t");
    Serial.print(F("z: "));
    Serial.println(int(z * 100) / 100.0);
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
    Serial.print(F("mag_x: ")); \
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

  // color sensor
  while (! APDS.colorAvailable()) {
    delay(5);
  }
  int r, g, b;

  // read the color
  APDS.readColor(r, g, b);
  // print the values
  Serial.println("Color Detection:");
  Serial.print("r = ");
  Serial.print(r);
  Serial.print(", g = ");
  Serial.print(g);
  Serial.print(", b = ");
  Serial.print(b);
  Serial.println();
//
//  if (APDS.gestureAvailable()) {
//    // a gesture was detected, read and print to serial monitor
//    int gesture = APDS.readGesture();
//
//    Serial.print("Proximity Detection: ");
//    switch (gesture) {
//      case GESTURE_UP:
//        Serial.println("Detected UP gesture");
//        break;
//
//      case GESTURE_DOWN:
//        Serial.println("Detected DOWN gesture");
//        break;
//
//      case GESTURE_LEFT:
//        Serial.println("Detected LEFT gesture");
//        break;
//
//      case GESTURE_RIGHT:
//        Serial.println("Detected RIGHT gesture");
//        break;
//
//      default:
//        // ignore
//        break;
//    }
//  }
  Serial.println();

}
