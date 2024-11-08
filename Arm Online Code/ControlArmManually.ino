//import the servo library
#include "Servo.h"

//Set up servos
Servo WristLAct;
Servo WristRAct;
Servo HandRPAct;
Servo HandIMAct;
Servo ForearmAct;
//set up pins
const int potWristPin = 0;
const int potHandPin = 1;
const int buttonWristPin=3;
const int wristLActPin=8;
const int wristRActPin=9;
const int handRPPin=5;
const int handIMPin=6;
const int forearmActPin=11;
const int forearmButton=4;

//values needed for calculations
int buttonForearmPressed=0;
int pvalue;
int actang;
int buttonWristPressed=0;
int time=0;
int wristTime=0;
void setup() 
{
  WristLAct.attach(wristLActPin);
  WristRAct.attach(wristRActPin);
  HandRPAct.attach(handRPPin);
  HandIMAct.attach(handIMPin);
  ForearmAct.attach(forearmActPin);
  ForearmAct.write(90);
  //(gave up, wasn't needed, need to add)
  //pinMode(potWristPin,INPUT);
  //pinMode(potHandPin,INPUT);
  //pinMode(buttonPin,INPUT);
  Serial.begin(9600);
}

void loop()
{  
//  HandMovement();
  WristMovement();
  ForearmMovement();
}

void WristMovement(){ //
  if(digitalRead(buttonWristPin)&&millis()>wristTime+200){  //digital debounce, counts button presses
    buttonWristPressed++;
    wristTime=millis();
  };
  pvalue = analogRead(potWristPin); //finds the potentionmeter value, maps to actuator, writes
  actang = map(pvalue, 0, 1023, 50, 140);
  WristLAct.write(actang);
  if (!(buttonWristPressed%2)){
    WristRAct.write(actang);
    } else{
      WristRAct.write(map(actang,50,140,140,50)); //reverse one direction to let the wrist turn
    }
  
}

void HandMovement(){  //Hand Movement
  pvalue = analogRead(potHandPin);
  actang = map(pvalue, 0, 1023, 20, 180);
  HandIMAct.write(actang);
  HandRPAct.write(map(actang,20,180,180,20)); //opposite bc the servo goes the other way
}

void ForearmMovement(){ //moves the forarm 90 when button pressed
  if(digitalRead(forearmButton)&&millis()>time+200){
      buttonForearmPressed++;
      if(buttonForearmPressed%2){
        ForearmAct.write(170);
      } else{
        ForearmAct.write(90);
      }
     time=millis();
  }
}
