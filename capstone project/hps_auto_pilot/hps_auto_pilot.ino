#include <Arduino_LSM9DS1.h>
#include <Arduino_LPS22HB.h>
#include <Arduino_HTS221.h>
#include <Arduino_APDS9960.h>
#include <ArduinoBLE.h>
//#include <stdlib.h>
//#include <AudioSound.h>
//#include <PDM.h>

#include <stdio.h>
unsigned long timer;
int str_size = 256;
BLEService sensorService("19B10000-E8F2-537E-4F6C-D104768A1214"); // BLE LED Service

// BLE LED Switch Characteristic - custom 128-bit UUID, read and writable by central
BLECharacteristic accelerometer("19B10000-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite, str_size, "Accelerometer");
BLECharacteristic gyroscope("19B10000-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite, str_size, "Gyroscope");
//BLECharacteristic acc_y("19B10000-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite, "");
//BLECharacteristic acc_z("19B10000-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite, "");
//
//BLECharacteristic gyro_x("19B10000-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite, "");
//BLECharacteristic gyro_y("19B10000-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite, "");
//BLECharacteristic gyro_z("19B10000-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite, "");



void setup() {
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }
  BLE.setLocalName("Adruino Sensors");
  BLE.setDeviceName("Arduino Sensors");

  BLE.setAdvertisedService(sensorService);

  // add the characteristic to the service
  sensorService.addCharacteristic(accelerometer);
  sensorService.addCharacteristic(gyroscope);
//  sensorService.addCharacteristic(acc_z);
//  sensorService.addCharacteristic(gyro_roll);
//  sensorService.addCharacteristic(gyro_pitch);
//  sensorService.addCharacteristic(gyro_yaw);

  // add service
  BLE.addService(sensorService);

  // start advertising
  BLE.advertise();

  Serial.println("Arduino NANO 33 BLE Sensors");

  // put your setup code here, to run once:
  //initialization
  Serial.begin(9600);
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU sensor!");
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
  String acc_data, gyro_data;
  float x, y, z;
  float yaw, pitch, roll;
  char acc_char[str_size];
  char gyro_char[str_size];
  BLEDevice central = BLE.central();
  //
  // if a central is connected to peripheral:
  if (central) {
    while (central.connected()) {
      delay(1000);
      Serial.println("connect successfully");
      //IMU
      if (IMU.accelerationAvailable()) {
        IMU.readAcceleration(x, y, z);
        acc_data = String(x, 2) + "," + String(y, 2) + "," + String(z, 2);
        acc_data.toCharArray(acc_char, str_size);
        accelerometer.writeValue(acc_char);

      }
  

      if (IMU.gyroscopeAvailable()) {
        IMU.readGyroscope(roll, pitch, yaw);
        //gyro_data = String(roll + ", " + pitch + ", " + yaw);
        //gyro_data = String(roll) + "," + String(pitch) + "," + String(yaw);
        //gyro_data.toCharArray(gyro_char, str_size);
        //gyroscope.writeValue(gyro_char);
      }

    }


    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }

  //  delay(1000);
  //  while (BLE.connected()) {
  //    Serial.println("Connection established");
  //
  //    // put your main code here, to run repeatedly:
  //    delay(1000);
  //    timer = millis();
  //    Serial.println(timer);
  ////
  ////    //pressure sensor
  ////    float pressure = BARO.readPressure();
  ////    Serial.print("Pressure = ");
  ////    Serial.print(pressure);
  ////    Serial.println(" kPa");
  //

  //

  //
  //    float mag_x, mag_y, mag_z;
  //
  //    if (IMU.magneticFieldAvailable()) {
  //
  //      IMU.readMagneticField(mag_x, mag_y, mag_z);
  //      Serial.print(F("mag x: ")); \
  //      Serial.print(mag_x);
  //      Serial.print(" \t");
  //      Serial.print(F("mag y: "));
  //      Serial.print(mag_y);
  //      Serial.print(" \t");
  //      Serial.print(F("mag z: "));
  //      Serial.print(mag_z);
  //      Serial.println("uT");
  //    }
  //  }
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
  /*
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
  /*
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

    // query the number  of bytes available
    int bytesAvailable = PDM.available();

    // read into the sample buffer
    int bytesRead = PDM.read(sampleBuffer, bytesAvailable);

    // 16-bit, 2 bytes per sample
    samplesRead = bytesRead / 2;
    Serial.println(sampleBuffer);
  */

}
