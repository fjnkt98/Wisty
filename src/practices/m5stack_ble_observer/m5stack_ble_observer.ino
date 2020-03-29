#include <M5Stack.h>
#include <BLEDevice.h>

uint8_t seq = 0xFF;
#define MID 0xFFFF

BLEScan* pBLEScan;

void setup(){
  M5.begin();

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setActiveScan(false);

  M5.Lcd.print("BLE Central Device Initialized.");
}

void loop(){
  bool found = false;
  float temp, humid;

  BLEScanResults foundDevices = pBLEScan->start(3);
  int count = foundDevices.getCount();

  for (int i = 0; i < count; i++) {
    BLEAdvertisedDevice d = foundDevices.getDevice(i);

    if (d.haveManufacturerData()) {
      std::string data = d.getManufacturerData();
      int manu = data[1] << 8 | data[0];
      if (manu == MID && seq != data[2]) {
        found = true;
        seq = data[2];
        temp = (float)(data[4] << 8 | data[3]) / 100;
        humid = (float)(data[6] << 8 | data[5]) / 100;
        M5.Lcd.fillScreen(BLACK);
        M5.Lcd.setCursor(0, 0);
        M5.Lcd.printf(">>> seq: %d, t: %.1f, h: %.1f\r\n", seq, temp, humid);
      }
    }
  }
}
