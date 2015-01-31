#include <Makeblock.h>
#include <SoftwareSerial.h>
#include <Wire.h>

class SZDIYStepMotor {
public:
  int dirPin;
  int stpPin;
  
  SZDIYStepMotor(int8_t port) {
    dirPin = mePort[port].s1;
    stpPin = mePort[port].s2;
    
    pinMode(dirPin, OUTPUT);
    pinMode(stpPin, OUTPUT);
  };
  
  void step(boolean dir,int steps)
  {
    digitalWrite(dirPin,dir);
    delay(10);
    for(int i=0;i<steps;i++){
      digitalWrite(stpPin, HIGH);
      delayMicroseconds(180);
      digitalWrite(stpPin, LOW);
      delayMicroseconds(180); 
    }
  }
  
};


/* Ultra Sonic and RGB LED module */
#define ULTRA_MAX_DISTANCE 90.0
#define ULTRA_MIN_DISTANCE 10.0
#define ULTRA_RANGE 80.0

class SZDIYUltrasonicSensor {
public:
  double val;
  MeUltrasonicSensor ultraSonic;
  MeRGBLed led;

  SZDIYUltrasonicSensor(uint8_t ultraSonicPort, uint8_t ledPort) {
    ultraSonic = MeUltrasonicSensor(ultraSonicPort);
    led = MeRGBLed(ledPort);
    led.setNumber(4);
  }
  
  void initRead() {
    val = this->readDistance();
    this->setIndicator(false, 1.0);
  }
  
  double readDistance() {
    return (ultraSonic.distanceCm(ULTRA_MAX_DISTANCE) - ULTRA_MIN_DISTANCE) / ULTRA_RANGE * 100.0;
  }
  
  void readUltraSonic() {
    double newVal = this->readDistance();
    if (newVal < 5) {
      return;
    }
    
    if (newVal > 0 && newVal < 100.0) {
      setIndicator(true, newVal/100.0);
    }
    else {
      setIndicator(false, 1);
    }
    
    val = newVal;
  }
  
  void setIndicator(boolean correct, double l) {
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

SZDIYStepMotor motor(PORT_1);
SZDIYUltrasonicSensor ultraSonic(PORT_4, PORT_3);

void setup() {
  Serial.begin(9600);
  
  ultraSonic.initRead();
}

void loop() {
  ultraSonic.readUltraSonic();
  
  
  
//  if (ultraSonic.val > 80) {
//    motor.step(1, 400);
//  }
//  else 
  if (ultraSonic.val > 60) {
    motor.step(1, 200);
  }
  else {
//  if (ultraSonic.val > 40) {
//  }
//  if (ultraSonic.val < 20) {
    motor.step(0, 200);
//  }
//  else {
//    motor.step(0, 400);
  }
}

