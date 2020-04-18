#include <Arduino_LSM9DS1.h>  //IMU
#include <Arduino_LPS22HB.h>  //Pressure Sensor
#include <Arduino_HTS221.h>   //Humidity & Temperature Sensor
#include <ArduinoBLE.h>       //BLE Library
#include <SPI.h>              //SPI interface
#include <SD.h>               //SD card management

#include <stdio.h>
unsigned long timer;
int str_size = 256;
BLEService sensorService("19B10000-E8F2-537E-4F6C-D104768A1214"); // BLE sensor Service

// BLE LED Switch Characteristic - custom 128-bit UUID, read and writable by central
// two characteristics, one for accelerometer and one for gyroscope
BLECharacteristic accelerometer("19B10000-E8F2-537E-4F6C-D104768A1215", BLERead | BLENotify, "Accelerometer: (x_placeholder, y_placeholder, z_placeholder)");
BLECharacteristic gyroscope("19B10000-E8F2-537E-4F6C-D104768A1216", BLERead | BLENotify, "Gyroscope: (roll_placeholder, pitch_placeholder, yaw_placeholder)");

File myFile;

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
    Serial.println("Failed to start BLE service!");

    while (1);
  }
  // initialize SD card connected to pin 10
  if (!SD.begin(10)) {
    Serial.println("Failed to initialize SD card!");
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
    Serial.println("connect successfully");
    // open text file named 'data.txt' for write
    // if file does not exist, it is created
    myFile = SD.open("data.txt", FILE_WRITE);
  
    // check if file is opened without issue
    if (myFile) {
      // print header between data sets
      myFile.println("============");
      myFile.println("NEW DATA SET");
      myFile.println("============");
      myFile.println("x,y,z,roll,pitch,yaw");
      Serial.println("write done");
    } else {
      // if the file didn't open, print an error:
      Serial.println("error opening data file");
    }
    
    while (central.connected()) {
      delay(1000);

      //IMU
      if (IMU.accelerationAvailable()) {
        IMU.readAcceleration(x, y, z);
        
        //String concatenation 
        acc_data = String("Accelerometer: (") + String(x) + "," + String(y) + "," + String(z) + String(")");
	     	//Print accelerometer data to SD card
        myFile.print(String(x) + "," + String(y) + "," + String(z) + ",");
        acc_data.toCharArray(acc_char, str_size);
        accelerometer.writeValue(acc_char);
      }
  
      if (IMU.gyroscopeAvailable()) {
        IMU.readGyroscope(roll, pitch, yaw);
        gyro_data = String("Gyroscope: (") + String(roll) + "," + String(pitch) + "," + String(yaw) + String(")");
		    //Print gyroscope data to SD card
        myFile.println(String(roll) + "," + String(pitch) + "," + String(yaw));
        gyro_data.toCharArray(gyro_char, str_size);
        gyroscope.writeValue(gyro_char);
      }
    }

    // when the central disconnects, print it out:
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }
  myFile.close();

}
