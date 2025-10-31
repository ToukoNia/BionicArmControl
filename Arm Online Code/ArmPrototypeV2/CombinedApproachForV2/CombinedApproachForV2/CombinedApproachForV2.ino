//import the servo library
#include "ESP32Servo.h"
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <EMGFilters.h>

//Wrist Variables
const byte servoPin[4] = { 4, 5, 2, 3 };
Servo wristL;
Servo wristR;
int leftWristAngle, rightWristAngle;

// IMU Setup
Adafruit_BNO055 bno = Adafruit_BNO055(55);
uint16_t measurement_delay_us = 65535;

unsigned long imuTime, imuPreviousTime;

float internalTime;

int newYaw, newPitch;
int goalYaw = 0, goalPitch = 0;
int pitching, yawing;

int lastYaw = 0, lastPitch = 0;
int error;
double iSumP = 0, iSumY = 0;

byte switchGyroButton=6;
bool buttonValue = 0, buttonFlag = 0;
int gyroSwitchPressed = 0;

byte switchArmButton=7;
bool buttonFlag2 = 0;
int armSwitchPressed = 0;

long timeTrack2=0;
//Elbow Variables
Servo elbowL;
Servo elbowR;
int leftElbowAngle=90, rightElbowAngle=90;
bool lastPressed=0;
//EMG variables
#define TIMING_DEBUG 1
#define NumOfSensors 2
#define sampl 20
#define EMG_THRESHOLD 20
static byte SensorInputPins[] = { A0, A1 };
static int SensorThresholds[] = { 300, 300 };  //Nia 36 and 20
  // emg filter only support "SAMPLE_FREQ_500HZ" or "SAMPLE_FREQ_1000HZ"
SAMPLE_FREQUENCY sampleRate = SAMPLE_FREQ_500HZ;
NOTCH_FREQUENCY humFreq = NOTCH_FREQ_50HZ;
EMGFilters EMGs[NumOfSensors];
int EMGvalues[NumOfSensors];

double average[NumOfSensors];
int sum[NumOfSensors];
int count;
int timeCount=0;
bool armMode;
int vertical=0;; int rotational=0;
unsigned long timeStamp, timeBudget, timeTrack, timeCripple;
void EMGController() {  //need to check in with maciek
  //current idea either if average[1] is within 10% of average 2 and both over THRESHOLD from average 0
  //or if average 1 2 and 3 are all over threshold from average 0, and all over threshold, trigger event 1, else event 2
  //combination of these for all 3 points?
  if (armMode){
    if (average[1] > (average[0]+EMG_THRESHOLD)) {
      elbowVert(-1);
    } else if (average[0] > (average[1]+EMG_THRESHOLD)) {
      elbowVert(1);
    }
  } else{
      if (average[1] > (average[0]+EMG_THRESHOLD)) {
        elbowRot(-1);
    } else if (average[0] > (average[1]+EMG_THRESHOLD)) {
      elbowRot(1);
    }
  }
}
//Elbow Code
void elbowVert(int pitch) {
  if ((leftElbowAngle - pitch <= 170) && (leftElbowAngle - pitch >= 10)) {
    leftElbowAngle = leftElbowAngle - pitch;
  } else {
    return;
  }
  if ((rightElbowAngle + pitch <= 170) && (rightElbowAngle + pitch >= 10)) {
    rightElbowAngle = (pitch + rightElbowAngle);
  } else {
    return;
  }
  elbowL.write(leftElbowAngle);
  elbowR.write(rightElbowAngle);
}

void elbowRot(int rot) {
  if ((leftElbowAngle + rot <= 170) && (leftElbowAngle + rot >= 10)) {
    leftElbowAngle = leftElbowAngle + rot;
  } else {
    return;
  }
  if ((rightElbowAngle + rot <= 170) && (rightElbowAngle + rot >= 10)) {
    rightElbowAngle = (rot + rightElbowAngle);
  } else {
    return;
  }
  elbowL.write(leftElbowAngle);
  elbowR.write(rightElbowAngle);
}

//Wrist Code
void gyroWrist() {
  imuTime = millis() / (1000000);
  imuPreviousTime = imuTime - (100 / (1000000));

  //imuDistances();
  //constructionSetup();
  /* Get a new sensor event */
  sensors_event_t event; 
  bno.getEvent(&event);
  int pitching = event.orientation.y;
  int yawing = -event.orientation.z+90;
  int newPitch = WristPID(1,0.3,0.3,pitching,lastPitch,&iSumP,goalPitch);  //1,0.3,0.3we love u xx <3
  int newYaw = WristPID(1,0.5,0.4,yawing,lastYaw,&iSumY,goalYaw);           // 1,0.5,0.4
  
  
  //int rolling = event.orientation.x;

  wristIK(newYaw,newPitch);
  lastYaw = newYaw;
  lastPitch = newPitch;
}

void wristIK(int yaw, int pitch) {
  leftWristAngle = -yaw + pitch + 90;
  leftWristAngle = max(0, min(leftWristAngle, 180));
  rightWristAngle = (yaw + pitch) + 90;
  rightWristAngle = max(0, min(rightWristAngle, 180));
  wristR.write(rightWristAngle);
  wristL.write(leftWristAngle);
}

int WristPID(double Kp, double Ki, double Kd, double input, int previous_input, double* iSum, int goal) {
  double error = input - goal;
  double difference = error - previous_input;
  *iSum = Ki * (*iSum + error);
  double output = Kp * error + *iSum - Kd * difference;
  if (output > 90) output = 90;
  if (output < -90) output = -90;
  return output;
} 
//Sets arm to default pos
void constructionSetup() {
  wristL.write(90);
  wristR.write(90);
  elbowL.write(90);
  elbowR.write(90);
}

//EMG Code
/*
bool trendChecker(bool direction){  //change this to check EMG values and if one is bigger hold it till stopped. At point where past two samples change the mode.
  if (trend!=direction){
      trendcounter=0;
      trend=direction;
  } else if (trendcounter<2){
    trendcounter++;
   else{
      return 1;
  }
  return 0;
}
*/
void readSensors() {
  for (int n = 0; n < NumOfSensors; n++) {
    EMGvalues[n] = analogRead(SensorInputPins[n]);
    EMGvalues[n] = EMGs[n].update(EMGvalues[n]);
    int envlope = sq(EMGvalues[n]);
    EMGvalues[n] = (envlope > SensorThresholds[n]) ? envlope : 0;
  }
}

void averaging() {
  for (int n = 0; n < NumOfSensors; n++) {
    sum[n] = sum[n] + EMGvalues[n];
    if (count == sampl) {
      average[n] = sum[n] / sampl;
      if (average[n] < 0) {
        average[n] = 0;
      }
      sum[n] = 0;
      if (n == NumOfSensors - 1) {
        count = 0;
      }
    }
  }
  count++;
}

void setup() {
  // put your setup code here, to run once:
  wristL.attach(servoPin[0]);
  wristR.attach(servoPin[1]);
  elbowL.attach(servoPin[2]);
  elbowR.attach(servoPin[3]);
  pinMode(switchGyroButton, INPUT_PULLUP);
  pinMode(switchArmButton,INPUT_PULLUP);

  constructionSetup();
  delay(1000);
  for (int n = 0; n < NumOfSensors; n++) {  // Setup for the sensors
    EMGs[n].init(sampleRate, humFreq, true, true, true);
  }
  Serial.begin(9600);
  Serial1.begin(9600);

  /* Initialise the sensor */
  if (!bno.begin()) {
    //Empty loop, if the code does nothing then the IMU is not working or has not been found
  }
  timeBudget = 1e6 / sampleRate;

  delay(1000);
  bno.setExtCrystalUse(true);
  delay(3000);
}

void loop() {
  // put your main code here, to run repeatedly:
  timeStamp = micros();
  readSensors();
  averaging();
  Serial.print(EMGvalues[0]); Serial.print(", "); Serial.println(EMGvalues[1]); 
  //Code for Enabling/disabling gyrowrist
  buttonValue = !digitalRead(switchGyroButton);
  if (buttonValue == 1 && buttonFlag == 0 && millis() > timeTrack + 250) {
    gyroSwitchPressed++;
    timeTrack = millis();
    buttonFlag = 1;
  } else if (buttonValue == 0 && buttonFlag == 1) {
    buttonFlag = 0;
  }

  if (gyroSwitchPressed % 2) {
    gyroWrist();
  }
  
  buttonValue = !digitalRead(switchArmButton);
  if (buttonValue == 1 && buttonFlag2 == 0 && millis() > timeTrack2 + 250) {
    armSwitchPressed++;
    timeTrack2 = millis();
    buttonFlag2 = 1;
  } else if (buttonValue == 0 && buttonFlag2 == 1) {
    buttonFlag2 = 0;
  }
  /*
  if (armSwitchPressed % 2) {
    if (lastPressed){
      Serial.println("Open");
      Serial1.println("Open");
    } else{
      Serial.println("Close");
      Serial1.println("Close");
    }
    lastPressed=!lastPressed;
    armSwitchPressed++;
  }
  */
  armMode=armSwitchPressed%2;
  //Code for EMG Control
  if (timeCount%2) {
    EMGController();
  }

  timeStamp = micros() - timeStamp;
  timeCount++;
  delayMicroseconds(500);
  // if more than timeBudget, the sample rate need to reduce to
  // SAMPLE_FREQ_500HZ
}
