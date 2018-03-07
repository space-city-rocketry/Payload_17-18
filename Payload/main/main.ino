#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>

#include "portmap.h"
#include "data_logger.h"
#include "mpu6050.h"
#include "multiplexer.h"

const static float kSeaLevelPressure = 1020;

DataLogger *logger;
PayloadData data;
unsigned long timestamp = millis();
int state = 0;

long cycle_time = 100;
long start_time = millis();
long currentTime = start_time;

Multiplexer mux(kMUX[0], kMUX[1], kMUX[2], kMUX[3]);

Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);


void setup() {
  Serial.begin(115200);
  Wire.begin(); //Join I2C bus as a master device.

  pinMode(kMosfet, OUTPUT);

  for (int i = 0; i < 6; i++)
    data.voltages.voltage_flex[i] = 0;

  for (int i = 0; i < 3; i++)
    data.voltages.voltage_coin[i] = 0;
}

void loop() {

  start_time = millis();

  switch (state) {

    case 0:
      //TODO: Wait for dashboard signal
      if (Serial.read() > -1) {
        Serial.println("Starting...");
        state = 50;

        if (!bmp.begin())
          Serial.println("BMP180 NOT DETECTED! PRESSURE AND ALTITUDE WILL NOT BE LOGGED!");

        Wire.beginTransmission(MPU6050_I2C_ADDRESS);
        if (Wire.endTransmission(false) == 2)
          Serial.println("MPU6050 NOT DETECTED! ACCELERATION WILL NOT BE LOGGED!");

        MPU6050_enable();

        digitalWrite(kMosfet, HIGH);

        logger = new DataLogger(&data, 0, "data.csv");
      }
      break;

    case 50:

      //Set the timestamp
      data.timestamp.time = millis();

      //Read, interpret, and log acceleration data from MPU6050
      int error;
      accel_t_gyro_union accel_t_gyro;

      error = MPU6050_read (MPU6050_ACCEL_XOUT_H, (uint8_t *) &accel_t_gyro, sizeof(accel_t_gyro));

      uint8_t swap;
      #define SWAP(x,y) swap = x; x = y; y = swap

      SWAP (accel_t_gyro.reg.x_accel_h, accel_t_gyro.reg.x_accel_l);
      SWAP (accel_t_gyro.reg.y_accel_h, accel_t_gyro.reg.y_accel_l);
      SWAP (accel_t_gyro.reg.z_accel_h, accel_t_gyro.reg.z_accel_l);

      data.accelerations.axis[0] = accel_t_gyro.value.x_accel / 16384.0 * 9.81;
      data.accelerations.axis[1] = accel_t_gyro.value.y_accel / 16384.0 * 9.81;
      data.accelerations.axis[2] = accel_t_gyro.value.z_accel / 16384.0 * 9.81;

      //Read, interpret, and log the voltage data from the batteries.
      for (int i = 0; i < 6; i++) {
        data.voltages.voltage_flex[i] = mux.multiplexerRead(kMuxOut, kFlexMUX[i]) / 1023.0 * 3.3;
      }

      for (int j = 0; j < 3; j++) {
        data.voltages.voltage_coin[j] = analogRead(kCoin[j]) / 1023.0 * 3.3;
      }

      //Read and log the pressure and altitude from the BMP180
      sensors_event_t event;
      bmp.getEvent(&event);

      if (event.pressure) {
        data.bmp.pressure = event.pressure;
        data.bmp.altitude = bmp.pressureToAltitude(kSeaLevelPressure, event.pressure);
      }

      //Read and log the temperatures from the DS18B20 temperature sensors
      for (int i = 0; i < 9; i++){
        data.temperatures.temperature[i] = mux.multiplexerRead(kMuxOut, kTemp[i]) / 1023.0 * 3.3 * 1000.0 / 10.0 * 2;
      }

      //Write data to SD card and check for shutdown signal.
      logger->writeToSD();

      //TODO: Check for shutdown signal
      if (Serial.read() > -1) {
        state = 100;
        Serial.println("Shutting down...");
        digitalWrite(kMosfet, LOW);
      }

      break;

    case 100:
      delete logger;
      MPU6050_disable();
      state = 0;
      break;
  }
  currentTime = millis();
  if (cycle_time - (currentTime - start_time) > 0)
    delay(cycle_time - (currentTime - start_time));
}
