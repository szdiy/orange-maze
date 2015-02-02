#include "Makeblock.h"
#include <SoftwareSerial.h>
#include <Wire.h>

#define KEY_START 1
#define KEY_NULL 0

long lastTime = 0;
int keystart = KEY_NULL;
int preset_time = 30; // the total time to set
int count_timer = preset_time; // the count down timer
int bri = 0, st = 0;
int suc_val; 

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
          keystart = KEY_START;
          Serial.println("KEY1 pressed");
          break;
        case KEY2:
          if(keystart != KEY_START)
          {
            preset_time = preset_time + 10;
          }
          if(preset_time > 300)
          {
            preset_time = 300;
          }
          count_timer = preset_time;
          Serial.println("KEY2 pressed");
          break;
        case KEY3:
          keystart = KEY_NULL;
          Serial.println("KEY3 pressed");
          count_timer = preset_time;
          break;
        case KEY4: 
          if(keystart != KEY_START)
          {
            preset_time = preset_time - 10;
            if(preset_time < 0)
            {
              preset_time = 0;
            }
          }
          count_timer = preset_time;
          Serial.println("KEY4 pressed");
          break;
        }
        key = newKey;
      }
    }
};

Me7SegmentDisplay disp(PORT_6);  //display
SZDIY4Button btn(PORT_8);        //keyboard
MeRGBLed led(PORT_3);
MePort sucout(PORT_3);

void setup()
{
  btn.initRead();
  led.setNumber(30);
  disp.display(0000);
  Serial.begin(9600);
}
void TimingISR()
{
  if(keystart == KEY_START)
  {
    count_timer = count_timer - 1;
  }
  if(count_timer == 0)
  {
    keystart = KEY_NULL;
  }
}

void displaytime()
{
  if(millis()-lastTime>=1000)
  {
    TimingISR();
    Serial.println(keystart,DEC);
    Serial.println((int)count_timer,DEC);
    disp.display(count_timer);
    lastTime = millis();
  }
}

void breathlight()
{
  if(bri>=100) st=1;
  if(bri<=0) st=0;
  
  if(st)bri--;
  else bri++;
  for(int t=0;t<30;t++){
    led.setColorAt(t, bri, bri, bri); // parameter description: led number, red, green, blue, flash mode
  }
  led.show();
  delay(10);
}

void loop()
{
  btn.read4Button();
  displaytime();
  if(keystart == KEY_NULL)
  {
    breathlight();
  }
  suc_val = sucout.dRead1();
  Serial.print("suc_val=");
  Serial.println(suc_val);
}

