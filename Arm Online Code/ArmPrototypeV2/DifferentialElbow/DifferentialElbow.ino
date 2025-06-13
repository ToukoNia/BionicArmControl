#include <Servo.h>
#include <Arduino.h>
const byte servoPin[2] = {4,5};

Servo elbowL;
Servo elbowR;
int leftAngle;
int rightAngle;

int pitch=45; int rot=90;
byte bUpPin=2, bDownPin=3, bLeftPin=4, bRightPin=5;
void setup() {
  // put your setup code here, to run once:
  elbowL.attach(10);
  elbowR.attach(11);
  Serial.begin(9600);
  pinMode(bUpPin,INPUT_PULLUP);
  pinMode(bDownPin,INPUT_PULLUP);
  pinMode(bLeftPin,INPUT_PULLUP);
  pinMode(bRightPin,INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:

  if (!digitalRead(bUpPin)){
    elbowVert(1);
  } else if (!digitalRead(bDownPin)){
    elbowVert(-1);
  }
  if (!digitalRead(bRightPin)){
    elbowRot(1);
  } else if (!digitalRead(bLeftPin)){
    elbowRot(-1);
  }
  delay(10);
}

void constructionSetup(){
  elbowL.write(90);
  elbowR.write(90);
}

void elbowVert(int pitch){
  if ((leftAngle-pitch<=180)&&(leftAngle-pitch>=0)){
    leftAngle=leftAngle-pitch;
  }  else{
    return;
  }if ((rightAngle+pitch<=180)&&(rightAngle+pitch>=0)){
    rightAngle=(pitch+rightAngle);  
  } else{
    return;
  }
  elbowL.write(leftAngle);
  elbowR.write(rightAngle);
}

void elbowRot(int rot){
  if ((leftAngle+rot<=360)&&(leftAngle+rot>=0)){
    leftAngle=leftAngle+rot;
  }  else{
    return;
  }if ((rightAngle+rot<=360)&&(rightAngle+rot>=0)){
    rightAngle=(rot+rightAngle);  
  } else{
    return;
  }
  elbowL.write(leftAngle);
  elbowR.write(rightAngle);
}

