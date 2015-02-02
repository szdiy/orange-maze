#include <Makeblock.h>
#include <SoftwareSerial.h>
#include <Wire.h>

Me4Button btn(PORT_6);
MePotentiometer myPotentiometer(PORT_7);
MeJoystick joystick(PORT_8);


int x = 0;      // a variable for the Joystick's x value
int y = 0;      // a variable for the Joystick's y value

void read4Button()
{
  switch(btn.pressed())//Check which button pressed
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
}

void readPotentiometer()
{
  Serial.print("value=");
  Serial.println(myPotentiometer.read());
}

void readJoystick()
{
  // read the both joystick axis values:
  x = joystick.readX();  
  y = joystick.readY(); 

  // print the results to the serial monitor:
  Serial.print("Joystick X = " );                       
  Serial.print(x);   
  Serial.print("\t Joystick Y = " );                       
  Serial.println(y);   
}

void setup() 
{
  Serial.begin(9600);
}

void loop()
{ 
  read4Button();
  readPotentiometer();
  readJoystick();
  
  delay(100);
}


