#include <MadgwickAHRS.h>
#include <M5StickC.h>

Madgwick madgwick;

float accX = 0;
float accY = 0;
float accZ = 0;

float gyroX = 0;
float gyroY = 0;
float gyroZ = 0;

float roll = 0;
float pitch = 0;
float yaw = 0;

const float gyro_x_offset = -2.583664;
const float gyro_y_offset = 0.442442;
const float gyro_z_offset = 4.964869;

unsigned long now = 0;
unsigned long old = 0;

void setup() {
  // put your setup code here, to run once:
  M5.begin();
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.print("Madgiwick Filter!");
  M5.MPU6886.Init();
  madgwick.begin(25);
}

void loop() {
  now = millis();

  if ((now - old) >= 40) {
    M5.MPU6886.getGyroData(&gyroX, &gyroY, &gyroZ);
    M5.MPU6886.getAccelData(&accX, &accY, &accZ);

    madgwick.updateIMU((gyroX - gyro_x_offset), (gyroY - gyro_y_offset) , (gyroZ - gyro_z_offset) , accX, accY, accZ);
    roll = madgwick.getRoll();
    pitch = madgwick.getPitch();
    yaw = madgwick.getYaw();
    Serial.print(roll);
    Serial.print("\t");
    Serial.print(pitch);
    Serial.print("\t");
    Serial.print(yaw);
    Serial.print("\t");
    Serial.println(now - old);
    
    M5.Lcd.setCursor(0, 10);
    M5.Lcd.printf("roll: %f", roll);
    M5.Lcd.setCursor(0, 20);
    M5.Lcd.printf("pitch: %f", pitch);
    M5.Lcd.setCursor(0, 30);
    M5.Lcd.printf("yaw: %f", yaw);

    old = now;
  }

  if (M5.BtnA.wasPressed()) {
    esp_restart();
  }

  M5.update();
}
