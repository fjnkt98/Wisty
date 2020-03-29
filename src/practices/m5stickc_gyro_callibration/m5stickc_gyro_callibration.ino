#include <M5StickC.h>

float accX = 0;
float accY = 0;
float accZ = 0;

float gyroX = 0;
float gyroY = 0;
float gyroZ = 0;

unsigned long now = 0;
unsigned long old = 0;
unsigned long start = 0;

double gyro_x_sum = 0;
double gyro_y_sum = 0;
double gyro_z_sum = 0;

unsigned long count = 0;

double gyro_x_ave = 0;
double gyro_y_ave = 0;
double gyro_z_ave = 0;

void setup() {
  M5.begin();
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.println("IMU TEST");
  
  M5.MPU6886.Init();

  now = millis();
  old = millis();
  start = millis();
}

void loop() {
  now = millis();

  if ((now - start) < 180000) {
    if ((now - old) > 40) {
      M5.MPU6886.getGyroData(&gyroX, &gyroY, &gyroZ);
      M5.MPU6886.getAccelData(&accX, &accY, &accZ);
  
      count++;

      gyro_x_sum = gyro_x_sum + gyroX;
      gyro_y_sum = gyro_y_sum + gyroY;
      gyro_z_sum = gyro_z_sum + gyroZ;

      gyro_x_ave = gyro_x_sum / count;
      gyro_y_ave = gyro_y_sum / count;
      gyro_z_ave = gyro_z_sum / count;
      
      M5.Lcd.setCursor(0, 10);
      M5.Lcd.printf("x average: %f", gyro_x_ave);
      M5.Lcd.setCursor(0, 20);
      M5.Lcd.printf("y average: %f", gyro_y_ave);
      M5.Lcd.setCursor(0, 30);
      M5.Lcd.printf("z average: %f", gyro_z_ave);
  
      old = now;
    }
  } else {
    M5.Lcd.setCursor(0, 50);
    M5.Lcd.print("Callibration finished.");
  }

  if (M5.BtnA.wasPressed()) {
    esp_restart();
  }

  M5.update();
}
