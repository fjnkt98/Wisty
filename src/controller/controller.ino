#include <M5StickC.h>
#include <MadgwickAHRS.h>

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLE2902.h>

// UUID definition
#define SERVICE_UUID "ee68c913-d61f-4973-ac40-55cbcf645709"
#define CHARACTERISTIC_UUID_TX "c73926a3-319f-4150-8faf-dd66bd0f1984"

// For communication control
BLECharacteristic *pCharacteristicTX;
bool deviceConnected = false;

// Communication data type definition
struct WistySignal {
  float alpha;
  float beta;
  float gamma;
};

// Communication data
struct WistySignal data;

// IMU filter instance
Madgwick madgwick;

// Accelerometer raw data 
float accX = 0;
float accY = 0;
float accZ = 0;

// Gyroscope raw data
float gyroX = 0;
float gyroY = 0;
float gyroZ = 0;

// Filered imu data
float roll = 0;
float pitch = 0;
float yaw = 0;

// Gyroscope offset value
const float gyro_x_offset = -2.583664;
const float gyro_y_offset = 0.442442;
const float gyro_z_offset = 4.964869;

// Clock
unsigned long now = 0;
unsigned long old = 0;

// Override callback functions
class WistyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
  }

  void onDisconnect(BLEServer *pServer) {
    deviceConnected = false;
  }
};

void setup() {
  M5.begin();
  M5.MPU6886.Init();
  madgwick.begin(25);

  BLEDevice::init("M5StickC");
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new WistyServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);

  pCharacteristicTX = pService->createCharacteristic(
                        CHARACTERISTIC_UUID_TX,
                        BLECharacteristic::PROPERTY_NOTIFY
                      );
  pCharacteristicTX->addDescriptor(new BLE2902());

  pService->start();
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->start();

  M5.Lcd.setRotation(3);
  M5.Lcd.setCursor(0, 0, 1);
  M5.Lcd.print("BLE Peripheral Initialized.");
}

void loop() {
  now = millis();
  
  if ((now - old) >= 40 && deviceConnected == true) {
    M5.MPU6886.getGyroData(&gyroX, &gyroY, &gyroZ);
    M5.MPU6886.getAccelData(&accX, &accY, &accZ);

    madgwick.updateIMU((gyroX - gyro_x_offset), (gyroY - gyro_y_offset) , (gyroZ - gyro_z_offset) , accX, accY, accZ);
    roll = madgwick.getRoll();
    pitch = madgwick.getPitch();
    yaw = madgwick.getYaw();

    data.alpha = roll / 180;
    data.beta = pitch / 90;
    data.gamma = yaw / 180;

    pCharacteristicTX->setValue((uint8_t*) &data, sizeof(WistySignal));
    pCharacteristicTX->notify();

    M5.Lcd.setCursor(0, 20);
    M5.Lcd.print("Send data:");
    M5.Lcd.setCursor(0, 30);
    M5.Lcd.printf("alpha: %3.2f", data.alpha);
    M5.Lcd.setCursor(0, 40);
    M5.Lcd.printf("beta: %3.2f", data.beta);
    M5.Lcd.setCursor(0, 50);
    M5.Lcd.printf("gamma: %3.2f", data.gamma);

    old = now;
  }

  if (M5.BtnA.wasPressed()) {
    esp_restart();
  }

  M5.update();
}
