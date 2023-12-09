#include <U8g2lib.h>

#include "Arduino_BMI270_BMM150.h"

#include <SPI.h>
#include <Wire.h>
#include <SD.h>

#define PANIC(msg)         \
  {                        \
    Serial.println((msg)); \
    while (1)              \
      ;                    \
  }
#define ASSERT(cond, msg) \
  if (!(cond))            \
    PANIC((msg));

File myFile;
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

const int chipSelect = 10;

struct IMUData
{
  float accel_x;
  float accel_y;
  float accel_z;
  float gyro_x;
  float gyro_y;
  float gyro_z;
  float mag_x;
  float mag_y;
  float mag_z;
};

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ;

  Serial.print("\n等着 \n");
  ASSERT(IMU.begin(), "你IMU炸了");
  ASSERT(SD.begin(chipSelect), "你SD卡死了");

  // 注意这儿得提前建一个txt
  myFile = SD.open("syy.txt", FILE_WRITE);

  ASSERT(myFile, "md打不开");
  Serial.println("Write to syy.txt...");
  myFile.print("预处理器运行时间：（不一定在每次编译时变化）");
  myFile.print(__DATE__);
  myFile.print(" ");
  myFile.println(__TIME__);
  u8g2.setI2CAddress(0x78);
  ASSERT(u8g2.begin(), "你屏幕烧了");
  u8g2.setFont(u8g2_font_profont12_mf);
}

void loop()
{
  delay(10);
  static IMUData imu_d;
  if (readIMU(&imu_d))
  {
    Serial.print("Accel:");
    Serial.print(imu_d.accel_x);
    Serial.print(",");
    Serial.print(imu_d.accel_y);
    Serial.print(",");
    Serial.print(imu_d.accel_z);
    Serial.print("  Gyro:");
    Serial.print(imu_d.gyro_x);
    Serial.print(",");
    Serial.print(imu_d.gyro_y);
    Serial.print(",");
    Serial.print(imu_d.gyro_z);
    Serial.print("  Mag:");
    Serial.print(imu_d.mag_x);
    Serial.print(",");
    Serial.print(imu_d.mag_y);
    Serial.print(",");
    Serial.println(imu_d.mag_z);

    myFile.print("Accel:");
    myFile.print(imu_d.accel_x);
    myFile.print(",");
    myFile.print(imu_d.accel_y);
    myFile.print(",");
    myFile.print(imu_d.accel_z);
    myFile.print("  Gyro:");
    myFile.print(imu_d.gyro_x);
    myFile.print(",");
    myFile.print(imu_d.gyro_y);
    myFile.print(",");
    myFile.print(imu_d.gyro_z);
    myFile.print("  Mag:");
    myFile.print(imu_d.mag_x);
    myFile.print(",");
    myFile.print(imu_d.mag_y);
    myFile.print(",");
    myFile.println(imu_d.mag_z);
  }
  u8g2.clearBuffer();
  u8g2.drawStr(0, 10, "I AM FUCKING ALIVE");
  u8g2.sendBuffer();
  if (Serial.available())
  {
    myFile.flush();
    Serial.read();
    Serial.println("flush");
  }
}

int readIMU(IMUData *d)
{
  if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable() && IMU.magneticFieldAvailable())
  {
    IMU.readAcceleration(d->accel_x, d->accel_y, d->accel_z);
    IMU.readGyroscope(d->gyro_x, d->gyro_y, d->gyro_z);
    IMU.readMagneticField(d->mag_x, d->mag_y, d->mag_z);
    return 1;
  }
  return 0;
}