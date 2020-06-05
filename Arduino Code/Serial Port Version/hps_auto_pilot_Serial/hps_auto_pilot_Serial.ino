#include <Arduino_LSM9DS1.h>  //IMU
#include <Arduino_LPS22HB.h>  //Pressure Sensor
#include <Arduino_HTS221.h>   //Humidity & Temperature Sensor
#include <Arduino_APDS9960.h> //Gesture, Light, Proximity Sensors
#include <MadgwickAHRS.h>
#include <SensorFusion.h>

unsigned long timer;
Madgwick filter;
int seconds = 0;
int minutes = 0;
int delay_freq = 1000; //change frequencies of output
float deltat;

void setup() {
  // put your setup code here, to run once:
  //initialization
  Serial.println(1);
  Serial.begin(9600);
  filter.begin(200);
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

}

void loop() {

  // main code here,  run repeatedly:

  //delay(delay_freq);  //if you want change the data rate
  int i = 0;
  float x_vel = 0, y_vel = 0, z_vel = 0;
  float x_dis = 0, y_dis = 0, z_dis = 0;
  while (1) {
    timer = millis();
    minutes = timer / 60000;
    seconds = (timer % 60000) / 1000;

    // code to printout the time
    //    Serial.print("Time: ");
    //    Serial.print(minutes);
    //    Serial.print(":");
    //    Serial.println(seconds);

    //pressure sensor
    //    float pressure = BARO.readPressure();
    //    Serial.print("Pressure = ");
    //    Serial.print(pressure);
    //    Serial.println(" kPa");

    //IMU
    float x_accel = 0, y_accel = 0, z_accel = 0;
    float tempx, tempy, tempz, tempyaw, temppitch, temproll, mag_x, mag_y, mag_z;
    int roll, pitch, yaw;
    double grav_x, grav_y, grav_z;
    x_dis = 0;
    y_dis = 0;
    z_dis = 0;
    unsigned long begin_time, time_diff;


    while ((seconds >= (millis() % 60000) / 1000)) {  //break every second
      begin_time = millis();
      IMU.readAcceleration(tempx, tempy, tempz);
      IMU.readGyroscope(temproll, temppitch, tempyaw);
      //IMU.readMagneticField(mag_x, mag_y, mag_z);
      time_diff = (millis() - begin_time);

      //Madgwick Filter
      filter.updateIMU(-temproll, temppitch, tempyaw, tempx, tempy, tempz);


      roll = (int)filter.getRoll();
      pitch = (int)filter.getPitch();
      yaw = (int)filter.getYaw();

      //calculate the gravity function  -- need improvements
      grav_x = sin(pitch * PI / 180.0) ;
      grav_y = sin(roll * PI / 180.0);

      if (abs(roll) > abs(pitch)) {
        grav_z = 1 - abs(grav_y);
      } else {
        grav_z = 1 - abs(grav_x);
      }


      x_accel = (x_accel + abs(tempx) - abs(grav_x)) / 2;
      y_accel = (y_accel + abs(tempy) - abs(grav_y)) / 2;
      z_accel = (z_accel + abs(tempz) - abs(grav_z)) / 2;

      //Use Seial port to test out values
      //      Serial.println("Orientation: " + String(roll) + String(" ") + String(pitch) + String(" ") + String(yaw));
      //      Serial.println("Gravity: " + String(grav_x) + String(" ") + String(grav_y) + String(" ") + String(grav_z));
      //      Serial.println("Acceleration: " + String(x_accel) + String(" ") + String(y_accel) + String(" ") + String(z_accel));

      //calculation for displacement
      x_dis = time_diff * 0.001 * x_vel + x_accel * 9.8 * sq(time_diff * 0.001) / 2;
      y_dis = time_diff * 0.001 * y_vel + y_accel * 9.8 * sq(time_diff * 0.001) / 2;
      z_dis = time_diff * 0.001 * z_vel + z_accel * 9.8 * sq(time_diff * 0.001) / 2;
      x_vel = x_vel + time_diff * 0.001 * x_accel * 9.8;
      y_vel = y_vel + time_diff * 0.001 * y_accel * 9.8;
      z_vel = z_vel + time_diff * 0.001 * z_accel * 9.8;

     //reset after 1 minuet
      if (seconds == 59) {
        seconds = 0;
      }
    }

    //serail port display calculated displaced.
    Serial.println(String(x_dis) + String(" ") + String(y_dis) + String(" ") + String(z_dis));

    //    //temperature
    //    float temperature = HTS.readTemperature();
    //    Serial.print("Temperature = ");
    //    Serial.print(temperature);
    //    Serial.println(" °C");
    //
    //    //humidity
    //    float humidity    = HTS.readHumidity();
    //    Serial.print("Humidity    = ");
    //    Serial.print(humidity);
    //    Serial.println(" %");

    //Equation of Motions updates every second


    //Serail Port ouputs the data
    //    Serial.println(String("Acceleration: ") + String(x_accel, 2) + ", " + String(y_accel, 2) + ", " + String(z_accel, 2) );
    //
    //    Serial.println(String("roll: ") + String(roll, 2));
    //    Serial.println(String("pitch: ") + String(pitch, 2));
    //    Serial.println(String("yaw: ") + String(yaw, 2));
    //    Serial.println();
  }
}
