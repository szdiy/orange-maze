#include <Makeblock.h>
#include <SoftwareSerial.h>
#include <Wire.h>


int dirPin = mePort[PORT_1].s1;//the direction pin connect to Base Board PORT1 SLOT1
int stpPin = mePort[PORT_1].s2;//the Step pin connect to Base Board PORT1 SLOT2

int nStepCount = 0;
bool bStepDir = 0;

MeJoystick joystick(PORT_8);
MeLimitSwitch limitSwitch(PORT_6); 
\
int x = 0;      // a variable for the Joystick's x value
int y = 0;      // a variable for the Joystick's y value

void setup()
{
	pinMode(dirPin, OUTPUT);
	pinMode(stpPin, OUTPUT);
}

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

void loop()
{
  x = joystick.readX();  
  y = joystick.readY(); 
  
  if(x > 750)
  {
    bStepDir = 1;
  }
  else if(x < 250)
  {
    bStepDir = 0;
  }
  
  if(y > 750)
  {
    bStepDir = 1;
  }
  else if(y < 250)
  {
    bStepDir = 0;
  }
  
  if(bStepDir)
  {
  	step(1,200);//run 200 step
    if(nStepCount<100)
      nStepCount++;
    else
      bStepDir = 0;
  }
  else
  {
    	step(0,200);
    if(nStepCount>0)
      nStepCount--;
    else
      bStepDir = 1;
  }
/*	delay(1000);
	step(0,200);
	delay(1000);*/
}
