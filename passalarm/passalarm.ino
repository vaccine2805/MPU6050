#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <SoftwareSerial.h>
#include <Wire.h>

unsigned long period = 1000; //ระยะเวลาที่ต้องการรอ
unsigned long last_time = 0; //ประกาศตัวแปรเป็น global เพื่อเก็บค่าไว้ไม่ให้ reset จากการวนloop
unsigned long check = 1000; //ระยะเวลารอReset
unsigned long reset_time = 0;


int but1 = 5;
int ad = 0;
int led1 = 2;
int rs = 0;
int buttonstate = 0;
int bz = 3;
int sta = 0;
int StatusAlert = 0;
Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(115200);
  pinMode(led1, OUTPUT);
  pinMode(bz, OUTPUT);
  pinMode(but1, INPUT);

  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
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
    if ( millis() - last_time >= period) { // ทำงานทุก 1 วิ
      if (rttx >= -0.05 && rttx <= 0.03) { // เช็กหากว่า rttx อยู่ใน range ให้นับ 1
        ad++;
      }
      else { // หากมีการขยับตัวโดยที่ยังไม่ครบ 30 วิ ให้เคลียค่า
        ad = 0;
      }
//            Serial.println(rttx);
//            Serial.println(ad);
      last_time = millis(); //เซฟเวลาปัจจุบันไว้เพื่อรอจนกว่า millis() จะมากกว่าตัวมันเท่า period
    }
    if (ad >= 30) { // เมื่อนิ่งเกิน 30 วินาที ให้ Interrupt
      //function Interrupt
      StatusAlert = true;
//      Serial.println("Alert");
      digitalWrite(led1, HIGH);
      digitalWrite(bz, HIGH);
    }
  }

  if (digitalRead(but1) == 1) {
    if (StatusAlert == 0) {
      StatusAlert = 1;
      digitalWrite(led1, HIGH);
      digitalWrite(bz, HIGH);
//            Serial.println("Alert");
    }
    else {
      if ( millis() - reset_time >= check) {
        rs++;
        reset_time = millis();
//                Serial.println(rs);
      }
      if (rs >= 5) {
        StatusAlert = 0;
        ad = 0;
        digitalWrite(led1, LOW);
        digitalWrite(bz, LOW);
        Serial.println("Reset");
        rs = 0;
        delay(1000);
      }
    }
  }
  Serial.println(String(a.acceleration.x) + "," + String(a.acceleration.y) + "," +String(a.acceleration.z) + "|" + String(g.gyro.x) + "," + String(g.gyro.y) + "," + String(g.gyro.z)+ "|" + String(StatusAlert));
} 
