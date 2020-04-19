#include <Arduino_LSM9DS1.h>  //IMU
#include <Arduino_LPS22HB.h>  //Pressure Sensor
#include <Arduino_HTS221.h>   //Humidity & Temperature Sensor
#include <ArduinoBLE.h>       //BLE Library

#include <stdio.h>
unsigned long timer;
int str_size = 256;
BLEService sensorService("19B10000-E8F2-537E-4F6C-D104768A1214"); // BLE sensor Service

// BLE LED Switch Characteristic - custom 128-bit UUID, read and writable by central
// two characteristics, one for accelerometer and one for gyroscope
BLECharacteristic accelerometer("19B10000-E8F2-537E-4F6C-D104768A1215", BLENotify, "Accelerometer: (x_placeholder, y_placeholder, z_placeholder)");
BLECharacteristic gyroscope("19B10000-E8F2-537E-4F6C-D104768A1216", BLENotify, "Gyroscope: (roll_placeholder, pitch_placeholder, yaw_placeholder)");

void setup() {
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
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }

  //set name to for the board 
  BLE.setLocalName("Adruino Sensors");
  BLE.setDeviceName("Arduino Sensors");

  BLE.setAdvertisedService(sensorService);

  // add the characteristic to the service
  sensorService.addCharacteristic(accelerometer);
  sensorService.addCharacteristic(gyroscope);

  // add service
  BLE.addService(sensorService);

  // start advertising
  BLE.advertise();

  Serial.println("Arduino NANO 33 BLE Sensors");
}

void loop() {
  String acc_data, gyro_data;
  float x, y, z;
  float yaw, pitch, roll;
  char acc_char[str_size];
  char gyro_char[str_size];
  BLEDevice central = BLE.central();
  
  // if a central is connected to peripheral:
  if (central) {
    while (central.connected()) {
      delay(1000);
      Serial.println(sensorService.characteristicCount());

      Serial.println("connect successfully");
      
      //IMU
      if (IMU.accelerationAvailable()) {
        IMU.readAcceleration(x, y, z);
        
        //String concatenation 
        acc_data = String("Accelerometer: (") + String(x) + "," + String(y) + "," + String(z) + String(")");
        acc_data.toCharArray(acc_char, str_size);
        accelerometer.writeValue(acc_char);
        Serial.println(acc_data);
      }
  
      if (IMU.gyroscopeAvailable()) {
        IMU.readGyroscope(roll, pitch, yaw);
        gyro_data = String("Gyroscope: (") + String(roll) + "," + String(pitch) + "," + String(yaw) + String(")");
        gyro_data.toCharArray(gyro_char, str_size);
        gyroscope.writeValue(gyro_char);
        Serial.println(gyro_data);
      }
    }

    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }

}
