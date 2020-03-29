#include <M5StickC.h>

#include "BLEDevice.h"

#define T_PERIOD 5
#define S_PERIOD 5

//#define SERVICE_UUID "e6c4bd66-c393-44d3-93a2-2d8e9af0c6f8"
//#define CHARACTERISTIC_UUID "e166ae41-572a-4dda-b7e5-a338999da9dc"

BLEAdvertising *pAdvertising;
uint8_t seq = 0;

uint16_t temp_data = 234;
uint16_t humid_data = 567;

void setAdvData(BLEAdvertising *pAdvertising) {
  uint16_t temp = temp_data;
  uint16_t humid = humid_data;

  BLEAdvertisementData oAdvertisementData = BLEAdvertisementData();
  oAdvertisementData.setFlags(0x06);

  std::string strServiceData = "";
  strServiceData += (char)0x08;
  strServiceData += (char)0xff;
  strServiceData += (char)0xff;
  strServiceData += (char)0xff;
  strServiceData += (char)seq;
  strServiceData += (char)(temp & 0xff);
  strServiceData += (char)((temp >> 8) & 0xff);
  strServiceData += (char)(humid &0xff);
  strServiceData += (char)((humid >> 8) & 0xff);

  oAdvertisementData.addData(strServiceData);
  pAdvertising->setAdvertisementData(oAdvertisementData);
}

void setup() {
  M5.begin();

  BLEDevice::init("M5StickC");
  BLEServer *pServer = BLEDevice::createServer();
  pAdvertising = pServer->getAdvertising();

  M5.Lcd.setRotation(3);
  M5.Lcd.setCursor(0, 0, 1);
  M5.Lcd.print("BLE Device Initialized.");
}

void loop() {
  setAdvData(pAdvertising);

  pAdvertising->start();
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.printf(">>> seq: %d, t: %.1f, h: %.1f\r\n", seq, (float)temp_data/100, (float)humid_data/100);
  delay(T_PERIOD * 1000);
  pAdvertising->stop();
  delay(S_PERIOD * 1000);

  seq++;
}
