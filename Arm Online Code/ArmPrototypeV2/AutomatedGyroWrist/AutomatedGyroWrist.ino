//import the servo library
#include "Servo.h"
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

// Servo Setup
const byte servoPin[2] = {4,5};
Servo wristL;
Servo wristR;
int leftAngle;
int rightAngle;

int jxval, jyval, jxangle, jyangle;

unsigned long timeStamp;
double timeIncr;

// IMU Setup
Adafruit_BNO055 bno = Adafruit_BNO055(55);
uint16_t measurement_delay_us = 65535;

unsigned long imuTime;
unsigned long imuPreviousTime;

float internalTime;

int newYaw;
int newPitch;
int goalYaw = 0;
int goalPitch = 0;


int lastYaw=0;
int lastPitch=0;
int error;
double iSumP=0, iSumY=0;


void setup() 
{


  wristL.attach(servoPin[0]);
  wristR.attach(servoPin[1]);
  
  constructionSetup();

  Serial.begin(9600);
  delay(50);  

  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

 Serial.println("Orientation Sensor Test"); Serial.println("");
  
  /* Initialise the sensor */
  if(!bno.begin())
  {
    //Empty loop, if the code does nothing then the IMU is not working or has not been found
  }
  
  delay(1000);
    
  bno.setExtCrystalUse(true);

  constructionSetup();
  internalTime = millis();
  constructionSetup();

  delay(3000);
  constructionSetup();
 
}

void loop() {
  

  imuTime = millis()/(10^6);
  imuPreviousTime = imuTime - (100/(10^6));

  //imuDistances();
  //constructionSetup();
  /* Get a new sensor event */ 
  sensors_event_t event; 
  bno.getEvent(&event);
  int pitching = event.orientation.y;
  int yawing = -event.orientation.z+90;
  int newPitch = WristPID(1.05,0.3,0.3,pitching,lastPitch,&iSumP,goalPitch);  //1,0.3,0.3we love u xx <3
  int newYaw = WristPID(1.05,0.5,0.4,yawing,lastYaw,&iSumY,goalYaw);           // 1,0.5,0.4
  
  
  //int rolling = event.orientation.x;

   wristIK(newYaw,newPitch);
  lastYaw = newYaw;
  lastPitch = newPitch;

   Serial.print("IMU-Pitch:");Serial.print(pitching);Serial.print("  IMU-Yaw:");Serial.print(yawing);
   Serial.print("  PID-IMU-Pitch:");Serial.print(newPitch);Serial.print("  PID-IMU-Yaw:");Serial.print(newYaw);
   Serial.print("  PID-IsumY:");Serial.print(iSumY);
 // constructionSetup();

//   if (switchEMGPressed%2){
//    ForearmMovement();
//  //   Serial.println("Forearm movement active");
//   } else {
//    HandMovement();
//   //  Serial.println("Hand movement active");
//   }

  //Serial.print(EMGvalues[0]); Serial.print(", "); Serial.println(EMGvalues[1]); 
 

  //if (TIMING_DEBUG) {
    // Serial.print(average[0]); Serial.print(", "); Serial.println(average[1]);
    //can output stuff here for debugging
  //}
  delayMicroseconds(500);
  // if more than timeBudget, the sample rate need to reduce to
  // SAMPLE_FREQ_500HZ
}


void constructionSetup(){
  wristL.write(90);
  wristR.write(90);
}

void wristIK(int yaw,int pitch){
  leftAngle=-yaw+pitch+90;
  leftAngle=max(0,min(leftAngle,180));
  rightAngle=(yaw+pitch)+90;
  rightAngle=max(0,min(rightAngle,180));
  wristL.write(leftAngle);
  wristR.write(rightAngle);
   Serial.print(" | leftAngle: ");
   Serial.print(leftAngle);
   Serial.print(" | rightAngle: ");
   Serial.println(rightAngle);
  // Serial.print(" | pitch: ");
  // Serial.print(pitchIK);
  // Serial.print(" | yaw: ");
  // Serial.println(yawIK);

}

int WristPID(double Kp, double Ki,double Kd,double input,int previous_input, double* iSum, int goal){
  double error = input - goal;
  double difference = error - previous_input;
  *iSum = Ki*(*iSum + error);
  double output = Kp * error + *iSum - Kd * difference;
  if(output>90) output=90;
  if(output<-90) output=-90; 
  return output;
}