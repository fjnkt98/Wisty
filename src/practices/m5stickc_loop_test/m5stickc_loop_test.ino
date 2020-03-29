#include <M5StickC.h>

unsigned long now = 0;
unsigned long old = 0;

void setup() {
  M5.begin();
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.print("Loop Test");

  now = millis();
  old = millis();
}

void loop() {
  now = millis();

  if ((now - old) >= 40) {
    M5.Lcd.setCursor(0, 10);
    M5.Lcd.printf("Interval: %d", (now - old));
    Serial.println(now - old);
    
    old = now;
  }
}
