#include <M5Stack.h>
#include <BLEDevice.h>

#include "m5stack_simple_servo.h"

M5StackSimpleServo servo1(2, 500, 2400);  // PWM channel 2
M5StackSimpleServo servo2(3, 500, 2400);  // PWM channel 3
M5StackSimpleServo servo3(4, 500, 2400);  // PWM channel 4
M5StackSimpleServo servo4(5, 500, 2400);  // PWM channel 5

// Inverse kinematics for 4wd mecanum wheel
class WistyIK {
  private: 
    float max_ang_vel_;   // Maximum angular velocity of the motor [rad/s]
    float wheel_radius_;  // Radius of mecanum wheel [mm]
    float tread_half_;    // Half of wheel tread [mm]
    float  motor_vel[4];  // Result of IK [rad/s]

  public:
    WistyIK()
    :max_ang_vel_(5.7596),
    wheel_radius_(22),
    tread_half_(50)
    {
    }

    void calcIK(float alpha, float beta, float gamma){
      float v_max = max_ang_vel_ * wheel_radius_;
      float signal_strength;
      float x_vel;
      float y_vel;
      float theta_vel;

      // Determine the signal strength
      if (fabs(alpha) >= fabs(beta) && fabs(alpha) >= fabs(gamma)) {
        signal_strength = fabs(alpha);
      } else if (fabs(beta) >= fabs(alpha) && fabs(beta) >= fabs(gamma)) {
        signal_strength = fabs(beta);
      } else if (fabs(gamma) >= fabs(alpha) && fabs(gamma) >= fabs(beta)) {
        signal_strength = fabs(gamma);
      } else {
        signal_strength = 0;
      }

      // If the signals are all zero, the motors will be stop
      if (alpha == 0 && beta == 0 && gamma == 0){
        for (int i = 0; i < 4; i++) {
          motor_vel[i] = 0.0;
        }
      } else {
        x_vel = (signal_strength * alpha * v_max) / (fabs(alpha) + fabs(beta) + fabs(gamma));
        y_vel = (signal_strength * beta * v_max) / (fabs(alpha) + fabs(beta) + fabs(gamma));
        theta_vel = (signal_strength * gamma * v_max) / (2 * tread_half_ * (fabs(alpha) + fabs(beta) + fabs(gamma)));

        motor_vel[0] = (- x_vel - y_vel - 2 * tread_half_ * theta_vel) / wheel_radius_;
        motor_vel[1] = (  x_vel - y_vel - 2 * tread_half_ * theta_vel) / wheel_radius_;
        motor_vel[2] = (  x_vel + y_vel - 2 * tread_half_ * theta_vel) / wheel_radius_;
        motor_vel[3] = (- x_vel + y_vel - 2 * tread_half_ * theta_vel) / wheel_radius_;
      }
    }

    int getMotorVel(uint8_t number){
      return (int)((1900 * motor_vel[number - 1]) / (2 * max_ang_vel_) + 1450);
    }
};

WistyIK wistyik;

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
struct WistySignal {
  float alpha;
  float beta;
  float gamma;
};

// Communication data
struct WistySignal data;

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

  wistyik.calcIK(data.alpha, data.beta, data.gamma);

  M5.Lcd.setCursor(0, 80);
  M5.Lcd.printf("Servo 1: %4d", wistyik.getMotorVel(1));
  servo1.writeMicroseconds(wistyik.getMotorVel(1));
  M5.Lcd.setCursor(0, 90);
  M5.Lcd.printf("Servo 2: %4d", wistyik.getMotorVel(2));
  servo2.writeMicroseconds(wistyik.getMotorVel(2));
  M5.Lcd.setCursor(0, 100);
  M5.Lcd.printf("Servo 3: %4d", wistyik.getMotorVel(3));
  servo3.writeMicroseconds(wistyik.getMotorVel(3));
  M5.Lcd.setCursor(0, 110);
  M5.Lcd.printf("Servo 4: %4d", wistyik.getMotorVel(4));
  servo4.writeMicroseconds(wistyik.getMotorVel(4));
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

  servo1.attach(16);  // servo1 attach to GPIO 16
  servo2.attach(17);  // servo2 attach to GPIO 17
  servo3.attach(5);   // servo3 attach to GPIO 5
  servo4.attach(26);  // servo4 attach to GPIO 26

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
