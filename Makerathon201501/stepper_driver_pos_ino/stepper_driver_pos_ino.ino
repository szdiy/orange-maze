/*************************************************************************
* File Name          : stepper_driver_pos.ino
* Author             : myan
* Version            : V1.0.0
* Date               : 01/31/2015
*Parts required	     :Me Stepper Driver , Stepper motor 
* Description        : Getting stared with  Me Stepper Driver V 1.0.

* License            : BSD
* Copyright (C) 2015 www.szdiy.org. All right reserved.
* http://www.szdiy.org/
**************************************************************************/

//Stepper Driver connection
//connect  	1A and 1B to stepper coil 1  nornally  black and green wire
//connect 	2A and 2B to stepper coil 2  nornally red and blue wire

#include <AccelStepper.h>
#include <Makeblock.h>
#include <SoftwareSerial.h>
#include <Wire.h>

int dirPin = mePort[PORT_1].s1;//the direction pin connect to Base Board PORT1 SLOT1
int stpPin = mePort[PORT_1].s2;//the Step pin connect to Base Board PORT1 SLOT2
AccelStepper stepper(AccelStepper::DRIVER,stpPin,dirPin); 

int y = 0;      // a variable for the Joystick's y value

MeJoystick joystick(PORT_7);

void setup()
{
  Serial.begin(9600);
  stepper.setMaxSpeed(10000);
  stepper.setAcceleration(20000);
  stepper.setCurrentPosition(0);
}

void loop()
{
  y = joystick.readY(); 
  // Change direction at the limits
  //if (stepper.distanceToGo() == 0)
//	stepper.moveTo(-stepper.currentPosition());

  long long pos = (y-499) * 20;
  stepper.runToNewPosition(pos);
}
