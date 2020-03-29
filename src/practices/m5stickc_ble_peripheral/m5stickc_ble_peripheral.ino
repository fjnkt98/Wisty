#include <M5StickC.h>

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLE2902.h>

// UUID definition
#define SERVICE_UUID "ee68c913-d61f-4973-ac40-55cbcf645709"
#define CHARACTERISTIC_UUID_TX "c73926a3-319f-4150-8faf-dd66bd0f1984"

// For communication control
BLECharacteristic *pCharacteristicTX;
bool device_connected = false;

// Communication data type definition
struct wistySignal {
  int alpha;
  int beta;
  int gamma;
};

// Communication data
struct wistySignal data;

// Clock
unsigned long now = 0;
unsigned long old = 0;

// Override callback functions
class WistyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    device_connected = true;
  }

  void onDisconnect(BLEServer *pServer) {
    device_connected = false;
  }
};

void setup() {
  M5.begin();

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

  data.alpha = 10;
  data.beta = 20;
  data.gamma = 30;

  M5.Lcd.setRotation(3);
  M5.Lcd.setCursor(0, 0, 1);
  M5.Lcd.print("BLE Peripheral Initialized.");
}

void loop() {
  now = millis();
  
  if ((now - old) >= 400 && device_connected == true) {
    pCharacteristicTX->setValue((uint8_t*) &data, sizeof(wistySignal));
    pCharacteristicTX->notify();

    M5.Lcd.setCursor(0, 20);
    M5.Lcd.print("Send data:");
    M5.Lcd.setCursor(0, 30);
    M5.Lcd.printf("alpha: %d", data.alpha);
    M5.Lcd.setCursor(0, 40);
    M5.Lcd.printf("beta: %d", data.beta);
    M5.Lcd.setCursor(0, 50);
    M5.Lcd.printf("gamma: %d", data.gamma);

    old = now;
  }
}