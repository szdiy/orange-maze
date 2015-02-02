/*************************************************************************
* File Name          : Indicators.ino
* Author             : Evan
* Updated            : Ander
* Version            : V0.4.1
* Date               : 27/03/2014
* Description        : Test for Makeblock Electronic modules of MeRGBLed and
                       LedStrip. 
                      . 
* License            : CC-BY-SA 3.0
* Copyright (C) 2013 Maker Works Technology Co., Ltd. All right reserved.
* http://www.makeblock.cc/
**************************************************************************/
#include "Makeblock.h"
#include <SoftwareSerial.h>
#include <Wire.h>

MeRGBLed led(PORT_3);

void setup()
{
  Serial.begin(9600);
  // set the led quantity.
  led.setNumber(15);
}
int count = 1;

void loop()
{
  count = (count+1) % 4; 
  
 // generates random numbers
  indicators(count,128,0,0);
  delay(1000);
}
void indicators(byte count,byte r,byte g,byte b){
  Serial.print("Led ");
  for(int x = 0; x < 4; ++x){
    if (count == x) {
      led.setColorAt(x,r,g,b);
      led.show();
      Serial.print(x);
      Serial.print(": on ");
    }
    else {
      led.setColorAt(x, 0, 0, 0);
      led.show();
      Serial.print(x);
      Serial.print(": off ");
    }
  }
  Serial.println();     
}
