#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
#include <MPU6050.h>

#include "portmap.h"
#include "data_logger.h"
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

MPU6050 accel;

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

        Serial.println("Initializing I2C devices...");
        
        Serial.println(bmp.begin() ? "BMP180 connection successful!" : "BMP180 connection failed! Pressure and altitude will not be logged!");
    
        accel.initialize();
        Serial.println(accel.testConnection() ? "MPU6050 connection successful!" : "MPU6050 connection failed! Acceleration will not be logged!");

        digitalWrite(kMosfet, HIGH);

        logger = new DataLogger(&data, 0, "data.csv");
      }
      break;

    case 50:

      int16_t ax, ay, az;

      accel.getAcceleration(&ax, &ay, &az);

      data.accelerations.axis[0] = ax / 16384.0 * 9.81;
      data.accelerations.axis[1] = ay / 16384.0 * 9.81;
      data.accelerations.axis[2] = az / 16384.0 * 9.81;

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
        data.temperatures.temperature[i] = ((mux.multiplexerRead(kMuxOut, kTemp[i]) * 3300 / 1023.0)-500)/10;
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
      state = 0;
      break;
  }
  currentTime = millis();
  if (cycle_time - (currentTime - start_time) > 0)
    delay(cycle_time - (currentTime - start_time));
}
