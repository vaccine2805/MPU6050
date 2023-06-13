#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

float  arrx[30];
int i = 0;
float Val_Ac = 0;
float i_count_Ave = 0;

Adafruit_MPU6050 mpu;

void setup(void) {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
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
  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  if (i > 29) {
    i = 0;
    Serial.println( "i == 0" );
  }
  else
  {
    Serial.print("i = ");
    Serial.print(i);
    Serial.print("  ==> ");
    arrx[i] = float(g.gyro.x);
    Serial.println(arrx[i]);
    i = i + 1;

    for (int i_count = 0; i_count < 30; i_count ++ )
    {
      Val_Ac = Val_Ac +  arrx[i_count];
      Serial.print("Val_Ac = ");
      Serial.println(Val_Ac);
    }
    i_count_Ave = Val_Ac / 30;
    Serial.print("i_count_Ave = ");
    Serial.println(i_count_Ave);

//    if ((- 0.05 > i_count_Ave ) && (0.05 < i_count_Ave )) 
    
    if ((- 0.03 == i_count_Ave )) 
    {
      digitalWrite(LED_BUILTIN, HIGH);
      Serial.println("LED : ON ///////////////////////////////////");
      delay(10);
    }
    else
    {
      digitalWrite(LED_BUILTIN, LOW);
      Serial.println("LED : OFF ");
      delay(10);
    }

    Val_Ac = 0;
    Serial.print("Val_Ac Reset = ");
    Serial.println(Val_Ac);
    i_count_Ave = 0;
    Serial.print("i_count_Ave Reset = ");
    Serial.println(i_count_Ave);


  }
  //Serial.println(String(a.acceleration.x) + "," + String(a.acceleration.y) + "," + String(a.acceleration.z) + "|" + String(g.gyro.x) + "," + String(g.gyro.y) + "," + String(g.gyro.z));
  delay(1);
}
