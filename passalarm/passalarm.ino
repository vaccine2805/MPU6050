#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <SoftwareSerial.h>
#include <Wire.h>

// HC-12 settings
SoftwareSerial hc12Serial(10, 11);  // HC-12 module connected to Arduino pins 10 (RX) and 11 (TX)

unsigned long period = 1000; // Time interval to wait
unsigned long last_time = 0; // Global variable to store the last time
unsigned long check = 1000; // Reset check interval
unsigned long reset_time = 0;

int but1 = 5;
int ad = 0;
int led1 = 2;
int led2 = 3;
int rs = 0;
int buttonstate = 0;
bool StatusAlert = false;
Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(9600);
  hc12Serial.begin(9600);  // HC-12 module baud rate

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(but1, INPUT);

  while (!Serial)
    delay(10); // Pause until the serial console opens

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize MPU6050
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
    case MPU6050_RANGE_2_G:
      Serial.println("+-2G");
      break;
    case MPU6050_RANGE_4_G:
      Serial.println("+-4G");
      break;
    case MPU6050_RANGE_8_G:
      Serial.println("+-8G");
      break;
    case MPU6050_RANGE_16_G:
      Serial.println("+-16G");
      break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
    case MPU6050_RANGE_250_DEG:
      Serial.println("+- 250 deg/s");
      break;
    case MPU6050_RANGE_500_DEG:
      Serial.println("+- 500 deg/s");
      break;
    case MPU6050_RANGE_1000_DEG:
      Serial.println("+- 1000 deg/s");
      break;
    case MPU6050_RANGE_2000_DEG:
      Serial.println("+- 2000 deg/s");
      break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
    case MPU6050_BAND_260_HZ:
      Serial.println("260 Hz");
      break;
    case MPU6050_BAND_184_HZ:
      Serial.println("184 Hz");
      break;
    case MPU6050_BAND_94_HZ:
      Serial.println("94 Hz");
      break;
    case MPU6050_BAND_44_HZ:
      Serial.println("44 Hz");
      break;
    case MPU6050_BAND_21_HZ:
      Serial.println("21 Hz");
      break;
    case MPU6050_BAND_10_HZ:
      Serial.println("10 Hz");
      break;
    case MPU6050_BAND_5_HZ:
      Serial.println("5 Hz");
      break;
  }
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  float rttx = g.gyro.x;
  
  if (StatusAlert == false) {
    if (millis() - last_time >= period) { // Execute every 1 second
      if (rttx >= -0.05 && rttx <= 0.00) { // Check if rttx is within the specified range
        ad++;
      }
      else {
        ad = 0;
      }
      last_time = millis(); // Save the current time
    }
    
    if (ad >= 30) {
      StatusAlert = true;
      Serial.println("Alert");
      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
      hc12Serial.println("Alert");  // Transmit "Alert" via HC-12
    }
  }

  if (digitalRead(but1) == 1) {
    if (StatusAlert == false) {
      StatusAlert = true;
      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
      hc12Serial.println("Alert");  // Transmit "Alert" via HC-12
    }
    else {
      if (millis() - reset_time >= check) {
        rs++;
        reset_time = millis();
      }
      if (rs >= 5) {
        StatusAlert = false;
        ad = 0;
        digitalWrite(led1, LOW);
        digitalWrite(led2, LOW);
        Serial.println("Reset");
        hc12Serial.println("Reset");  // Transmit "Reset" via HC-12
        rs = 0;
        delay(1000);
      }
    }
  }
  
  // Transmit accelerometer and gyroscope data via HC-12
  String data = String(a.acceleration.x) + "," + String(a.acceleration.y) + "," + String(a.acceleration.z) + "|" + String(g.gyro.x) + "," + String(g.gyro.y) + "," + String(g.gyro.z);
  hc12Serial.println(data);
  Serial.println(data);
}
