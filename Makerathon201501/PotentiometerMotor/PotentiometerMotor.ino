#include <Makeblock.h>
#include <SoftwareSerial.h>
#include <Wire.h>


/* 4Button Module */
class SZDIY4Button: public Me4Button {
  public:
    int key;
    
    SZDIY4Button(uint8_t port):Me4Button(port) {
    }
    
    void initRead() {
      key = this->pressed();
    }
    
    void read4Button() {
      uint8_t newKey = this->pressed();
      if (newKey != key) { // on key changed
      
        switch(newKey)//Check which button pressed
        {
        case KEY1:
          Serial.println("KEY1 pressed");
          break;
        case KEY2:
          Serial.println("KEY2 pressed");
          break;
        case KEY3:
          Serial.println("KEY3 pressed");
          break;
        case KEY4:
          Serial.println("KEY4 pressed");
          break;
        }
        
        key = newKey;
      }
    }
};


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

/* Potentiometer Module */
class SZDIYPotentiometer: public MePotentiometer {
  public:
    double val; // normal
    
  
    SZDIYPotentiometer(uint8_t port): MePotentiometer(port) {}
  
    void initRead() {  
      val = this->read() / 10.0;
    }
    
    void readPotentiometer() {
      
      uint8_t newVal = this->read() / 10.0;
      if (val != newVal) {
        Serial.print("value=");
        Serial.println(newVal);
        
        val = newVal;
      }
    }
};

SZDIYStepMotor motor(PORT_1);
SZDIYPotentiometer meter(PORT_7);

void setup() {
  Serial.begin(9600);
  
  meter.initRead();
}

void loop() {
  meter.readPotentiometer();
  
  if (meter.val > 55) {
    motor.step(1, 200);
  }
  if (meter.val < 45) {
    motor.step(0, 200);
  }
    
}


