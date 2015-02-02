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
    
    Serial.print("measure: ");
    Serial.println(newVal);
    
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


SZDIY4Button btn(PORT_6);
SZDIYPotentiometer meter(PORT_7);
SZDIYJoystick joystick(PORT_8);
SZDIYUltrasonicSensor ultraSonic(PORT_4, PORT_3);

void setup() 
{
  Serial.begin(9600);
  
  btn.initRead();
//  meter.initRead();
//  joystick.initRead();
//  ultraSonic.initRead();
}

void loop()
{ 
  btn.read4Button();
//  meter.readPotentiometer();
//  joystick.readJoystick();
//  ultraSonic.readUltraSonic();

  switch(btn.key)//Check which button pressed
  {
  case KEY1:
    buzzerOn();
    break;
  case KEY2:
    buzzerOff();
    break;
  case KEY3:
    buzzerOff();
    break;
  case KEY4:
    buzzerOn();
    break;
  }
  
  delay(10);
}

