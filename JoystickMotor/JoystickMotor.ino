#include <Makeblock.h>
#include <SoftwareSerial.h>
#include <Wire.h>


class SZDIYJoystick: public MeJoystick {
  public:
    SZDIYJoystick(uint8_t port): MeJoystick(port) {}
  
    int x;
    int y;
    
    void initRead() {
      x = this->readX();
      y = this->readY();
    }
    
    void readJoystick() {
 
      // read the both joystick axis values:
      int new_x = this->readX();  
      int new_y = this->readY(); 
    
      // print the results to the serial monitor:
      if (x != new_x || y != new_y) {
        Serial.print("Joystick X = " );                       
        Serial.print(new_x);   
        
        Serial.print("\t Joystick Y = " );                       
        Serial.println(new_y);
        
        x = new_x;
        y = new_y;
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
SZDIYJoystick joystick(PORT_8);

void setup() {
  Serial.begin(9600);
  
  joystick.initRead();
}

void loop() {
  joystick.readJoystick();
  delay(10);
  if (joystick.y > 600) {
     motor.step(0, 200);
  }
  else if (joystick.y > 400) {
    
  } else {
    motor.step(1, 200);
  }
}


