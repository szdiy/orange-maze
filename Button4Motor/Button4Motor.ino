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

SZDIYStepMotor motor(PORT_1);
SZDIY4Button btn(PORT_6);

void setup() {
  Serial.begin(9600);
  
  btn.initRead();
}

void loop() {
  btn.read4Button();
  
  switch(btn.key) {
    case KEY1:
      motor.step(1, 200);
      break;
    case KEY2:
      motor.step(1, 200);
      break;
    case KEY3:
      motor.step(0, 200);
      break;
    case KEY4:
      motor.step(0, 200);
      break;
  }
}


