//import the servo library
#include "ESP32Servo.h"
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <EMGFilters.h>
#define TIMING_DEBUG 1
#define NumOfSensors 2
#define sampl 20


void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}

class Wrist{
  private:
    Servo wristL;
    Servo wristR;
    int leftWristAngle, rightWristAngle;
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

    void WristPID(double Kp, double Ki, double Kd, double input, int previous_input, double* iSum, int goal)
    void wristIK(int yaw, int pitch);
  public:
    void ServoSetup();
    void gyroWrist(int goalPitch, int goalYaw);
    void constructionSetup();
}

void Wrist::WristPID(double Kp, double Ki, double Kd, double input, int previous_input, double* iSum, int goal){
  double error = input - goal;
  double difference = error - previous_input;
  *iSum = Ki * (*iSum + error);
  double output = Kp * error + *iSum - Kd * difference;
  if (output > 90) output = 90;
  if (output < -90) output = -90;
  return output;
}

void Wrist::wristIK(int yaw, int pitch) {
  leftWristAngle = -yaw + pitch + 90;
  leftWristAngle = max(0, min(leftWristAngle, 180));
  rightWristAngle = (yaw + pitch) + 90;
  rightWristAngle = max(0, min(rightWristAngle, 180));
  wristR.write(rightWristAngle);
  wristL.write(leftWristAngle);
}

void Wrist::constructionSetup(){
  wristR.write(90);
  wristL.write(90);
}

void Wrist::ServoSetup(byte wristLPin, byte wristRPin){
  wristL.attach(wristLPin);
  wristR.attach(wristRPin);
}

void Wrist::gyroWrist(int goalPitch, int goalYaw){
  imuTime = millis() / (1000000);
  imuPreviousTime = imuTime - (100 / (1000000));
  int pitching = event.orientation.y;
  int yawing = -event.orientation.z+90;
  int newPitch = WristPID(1,0.3,0.3,pitching,lastPitch,&iSumP,goalPitch);  //1,0.3,0.3we love u xx <3
  int newYaw = WristPID(1,0.5,0.4,yawing,lastYaw,&iSumY,goalYaw);           // 1,0.5,0.4
  wristIK(newYaw,newPitch);
  lastYaw = newYaw;
  lastPitch = newPitch;
}

class EMGController{
  private:
    int EMG_THRESHOLD = 20;
    byte SensorInputPins[];
    int SensorThresholds[] = { 300, 300 };
    SAMPLE_FREQUENCY sampleRate = SAMPLE_FREQ_500HZ;
    NOTCH_FREQUENCY humFreq = NOTCH_FREQ_50HZ;
    EMGFilters EMGs[NumOfSensors];
    int EMGvalues[NumOfSensors];
    double average[NumOfSensors];
    int sum[NumOfSensors];

    //bool PostProc(bool direction);
    void averaging();
  public:
    int EMGController();
    void readSensors();
}

int EMGController::EMGController() {  //need to check in with maciek
  //current idea either if average[1] is within 10% of average 2 and both over THRESHOLD from average 0
  //or if average 1 2 and 3 are all over threshold from average 0, and all over threshold, trigger event 1, else event 2
  //combination of these for all 3 points?
  if (armMode){
    if (average[1] > (average[0]+EMG_THRESHOLD)) {
      return 1;
    } else if (average[0] > (average[1]+EMG_THRESHOLD)) {
      return -1;
    }
  } else{
      if (average[1] > (average[0]+EMG_THRESHOLD)) {
        return 2;
    } else if (average[0] > (average[1]+EMG_THRESHOLD)) {
      return -2;
    }
  }
}

void EMGController::readSensors() {
  for (int n = 0; n < NumOfSensors; n++) {
    EMGvalues[n] = analogRead(SensorInputPins[n]);
    EMGvalues[n] = EMGs[n].update(EMGvalues[n]);
    int envlope = sq(EMGvalues[n]);
    EMGvalues[n] = (envlope > SensorThresholds[n]) ? envlope : 0;
  }
  averaging();
}

void EMGController::averaging() {
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
