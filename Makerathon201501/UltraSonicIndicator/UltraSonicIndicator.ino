#include "Makeblock.h"
#include <SoftwareSerial.h>
#include <Wire.h>

class SZDIYUltrasonicSensor {
public:
  uint8_t val;
  MeUltrasonicSensor ultraSonic;
  MeRGBLed led;

  SZDIYUltrasonicSensor(uint8_t ultraSonicPort, uint8_t ledPort) {
    ultraSonic = MeUltrasonicSensor(ultraSonicPort);
    led = MeRGBLed(ledPort);
    led.setNumber(4);
  }
  
  void initRead() {
    val = ultraSonic.distanceCm() / 10;
    this->setIndicator(false, 1.0);
  }
  
  void readUltraSonic() {
    uint8_t newVal = ultraSonic.distanceCm() / 10;
    if (newVal == 0) {
      return;
    }
    
    Serial.print("measure: ");
    Serial.println(newVal);
    
    if (newVal >= 1 && newVal < 10) {
      setIndicator(true, (float)(newVal)/16.0);
    }
    else {
      setIndicator(false, 1);
    }
    
    val = newVal;
  }
  
  void setIndicator(boolean correct, float l) {
      for (int i = 0; i < 4; i++) {
        if (correct) {
          led.setColorAt(i, 0, 128*l, 0);
        }
        else {
          led.setColorAt(i, 128, 0, 0);
        }
      }
      led.show();
  }
  
};

SZDIYUltrasonicSensor ultra(PORT_4, PORT_3);

void setup() {
  Serial.begin(9600);

  ultra.initRead();
}

void loop() {
  
  ultra.readUltraSonic();
  
  delay(10);
}

