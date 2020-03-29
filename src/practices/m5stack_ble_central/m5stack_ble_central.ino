#include <M5Stack.h>
#include <BLEDevice.h>

// UUID definition
BLEUUID SERVICE_UUID("ee68c913-d61f-4973-ac40-55cbcf645709");
BLEUUID CHARA_UUID_RX("c73926a3-319f-4150-8faf-dd66bd0f1984");

// For controling the communication
BLERemoteCharacteristic* pRemoteCharacteristicRX;
BLEAdvertisedDevice* targetDevice;

bool doConnect = false;
bool doScan = false;
bool deviceConnected = false;

// Communication data type definition
struct wistySignal {
  float alpha;
  float beta;
  float gamma;
};

// Communication data
struct wistySignal data;

// connect/disconnect callback functions
class WistyClientCallbacks : public BLEClientCallbacks {
  void onConnect(BLEClient* pClient) {
    ;
  }

  void onDisconnect(BLEClient* pClient) {
    deviceConnected = false;
  }
};

class AdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
  
    if (advertisedDevice.haveServiceUUID()) {
      BLEUUID service = advertisedDevice.getServiceUUID();

      if (service.equals(SERVICE_UUID)) {
        BLEDevice::getScan()->stop();
        targetDevice = new BLEAdvertisedDevice(advertisedDevice);
        doConnect = doScan = true;

        M5.Lcd.setCursor(0, 30);
        M5.Lcd.printf("Advertised Device found: %s", advertisedDevice.toString().c_str());
      }
    }
  }
};

static void notifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic,
                          uint8_t* pData, size_t length, bool isNotify) {
  memcpy(&data, pData, length);

  M5.Lcd.setCursor(0, 80);
  M5.Lcd.setTextSize(3);
  M5.Lcd.printf("roll: %3.2f", data.alpha);
  M5.Lcd.setCursor(0, 110);
  M5.Lcd.setTextSize(3);
  M5.Lcd.printf("pitch: %3.2f", data.beta);
  M5.Lcd.setCursor(0, 140);
  M5.Lcd.setTextSize(3);
  M5.Lcd.printf("yaw: %3.2f", data.gamma);
}

bool doPrepare() {
  BLEClient* pClient = BLEDevice::createClient();
  pClient->setClientCallbacks(new WistyClientCallbacks());

  pClient->connect(targetDevice);

  BLERemoteService* pRemoteService = pClient->getService(SERVICE_UUID);

  if (pRemoteService == nullptr) {
    M5.Lcd.setCursor(0, 30);
    M5.Lcd.print("Failed to find the service UUID");
    pClient->disconnect();
    return false;
  }

  pRemoteCharacteristicRX = pRemoteService->getCharacteristic(CHARA_UUID_RX);
  if (pRemoteCharacteristicRX == nullptr) {
    M5.Lcd.setCursor(0, 30);
    M5.Lcd.print("Failed to find the characteristic UUID");
    pClient->disconnect();
    return false;
  }

  if (pRemoteCharacteristicRX->canNotify()) {
    pRemoteCharacteristicRX->registerForNotify(notifyCallback);
  }

  deviceConnected = true;
  return true;
}

void setup() {
  M5.begin();
  BLEDevice::init("");

  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new AdvertisedDeviceCallbacks());

  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.print("BLE Central Initialized.");

  M5.Lcd.setCursor(0, 10);
  M5.Lcd.print("Scan start...");
  pBLEScan->setActiveScan(true);
  pBLEScan->start(10);
}

void loop() {
  if (doConnect == true) {
    if (doPrepare()) {
      M5.Lcd.setCursor(0, 20);
      M5.Lcd.print("Connected to the BLE peripheral.");
    } else {
      M5.Lcd.setCursor(0, 20);
      M5.Lcd.print("Failed to connect to the BLE peripheral.");
    }
    doConnect = false;
  }

  if (deviceConnected) {
    ;
  } else if (doScan) {
    BLEDevice::getScan()->start(0);
  }
}