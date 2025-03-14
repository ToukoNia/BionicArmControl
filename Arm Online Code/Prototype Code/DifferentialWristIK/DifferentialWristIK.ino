#include <Servo.h>
#include <Arduino.h>
const byte servoPin[2] = {4,5};

Servo wristL;
Servo wristR;
int leftAngle;
int rightAngle;

void setup() {
  // put your setup code here, to run once:
  wristL.attach(servoPin[0]);
  wristR.attach(servoPin[1]);
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  constructionSetup();
}

void constructionSetup(){
  wristL.write(90);
  wristR.write(90);
}

void wristIK(int pitch,int yaw){
  leftAngle=pitch+yaw;
  rightAngle=pitch-yaw;
  wristL.write(map(leftAngle,-90,90,0,180));
  wristR.write(map(leftAngle,-90,90,0,180));
}
