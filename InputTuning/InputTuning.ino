#include <Makeblock.h>
#include <SoftwareSerial.h>
#include <Wire.h>

class SZDIY4Button: public Me4Button {
  public:
    uint8_t key;
    
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

class SZDIYPotentiometer: public MePotentiometer {
  public:
    uint8_t val;
  
    SZDIYPotentiometer(uint8_t port): MePotentiometer(port) {}
  
    void initRead() {  
      val = this->read() / 100;
    }
    
    void readPotentiometer() {
      
      uint8_t newVal = this->read() / 100;
      if (val != newVal) {
        Serial.print("value=");
        Serial.println(newVal);
        
        val = newVal;
      }
    }
};

class SZDIYJoystick: public MeJoystick {
  public:
    SZDIYJoystick(uint8_t port): MeJoystick(port) {}
  
    int x;
    int y;
    
    void initRead() {
      x = this->readX() / 100;
      y = this->readY() / 100;
    }
    
    void readJoystick() {
 
      // read the both joystick axis values:
      int8_t new_x = this->readX() / 100;  
      int8_t new_y = this->readY() / 100; 
    
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

SZDIY4Button btn(PORT_6);
SZDIYPotentiometer meter(PORT_7);
SZDIYJoystick joystick(PORT_8);

void setup() 
{
  Serial.begin(9600);
  
//  btn.initRead();
  meter.initRead();
  joystick.initRead();
  
}

void loop()
{ 
//  btn.read4Button();
  meter.readPotentiometer();
  joystick.readJoystick();
  
  delay(10);
}

