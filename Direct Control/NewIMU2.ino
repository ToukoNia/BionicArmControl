
//import the servo library
#include "Servo.h"
#include <EMGFilters.h>
#include <Arduino.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>




//Set up servos
Servo WristLAct;
Servo WristRAct;
Servo HandRPAct;
Servo HandIMAct;
Servo ForearmAct;
//set up pins

const int wristLActPin=8;
const int wristRActPin=9;

static int joyWristPin[] = {A0, A1};

const byte servoPin[2] = {4,5};
Servo wristL;
Servo wristR;
int leftAngle;
int rightAngle;

const int handRPPin=5;
const int handIMPin=6;
const int forearmActPin=11;
const int switchEMGButton=20;
const int EMG_THRESHOLD=20;

static byte SensorInputPins[] = { A2, A3};
static int SensorThresholds[] = { 0, 0}; //Nia 36 and 20

// emg filter only support "SAMPLE_FREQ_500HZ" or "SAMPLE_FREQ_1000HZ"
int sampleRate = SAMPLE_FREQ_500HZ;
// emg filter only support "NOTCH_FREQ_50HZ" or "NOTCH_FREQ_60HZ"
int humFreq = NOTCH_FREQ_50HZ;

int jxval, jyval, jxangle, jyangle;

#define TIMING_DEBUG 1
#define NumOfSensors 2

//values needed for calculations
int switchEMGPressed=0;
int pvalue;
int actang;
long int time=0;
int wristTime=0;
int timeCount = 0;


int actHandAng=20;
int actForearmAng=90;
#define sampl 20
int count;
int buttonValue = 0;
int buttonFlag = 0;

EMGFilters EMGs[NumOfSensors];
int EMGvalues[NumOfSensors];

double average[NumOfSensors];
int sum[NumOfSensors];
int wristRAng=85; int wristLAng=85;
int jxincr=0; int jyincr=0;

unsigned long timeStamp;
unsigned long timeBudget;

Adafruit_BNO055 bno = Adafruit_BNO055(55);
uint16_t measurement_delay_us = 65535;

unsigned long imuTime;
unsigned long imuPreviousTime;



float internalTime;

int newYaw;
int newPitch;
int goalYaw = 90;
int goalPitch = 0;


void setup() 
{
  WristLAct.attach(wristLActPin);
  WristRAct.attach(wristRActPin);
  HandRPAct.attach(handRPPin);
  HandIMAct.attach(handIMPin);
  ForearmAct.attach(forearmActPin);
 // ForearmAct.write(90);

  wristL.attach(servoPin[0]);
  wristR.attach(servoPin[1]);
  //(gave up, wasn't needed, need to add)
  //pinMode(potWristPin,INPUT);
  //pinMode(potHandPin,INPUT);
  //pinMode(buttonPin,INPUT);
  

  for (int n = 0; n < NumOfSensors; n++) { // Setup for the sensors
    EMGs[n].init(sampleRate, humFreq, true, true, true);
  }
  Serial.begin(9600);
  timeBudget = 1e6 / sampleRate;
  pinMode(switchEMGButton, INPUT_PULLUP);
  delay(50);  

  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

 Serial.println("Orientation Sensor Test"); Serial.println("");
  
  /* Initialise the sensor */
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  
  delay(1000);
    
  bno.setExtCrystalUse(true);

  constructionSetup();
  internalTime = millis();
  delay(3000);




}

void loop() {

  timeStamp = micros();  

  imuTime = millis()/(10^6);
  imuPreviousTime = imuTime - (100/(10^6));

  //imuDistances();
  //constructionSetup();
  /* Get a new sensor event */ 
  sensors_event_t event; 
  bno.getEvent(&event);
  int pitching = event.orientation.y;
  int newPitch = pitching - goalPitch;
  int yawing = event.orientation.z;
  int newYaw = 1.5*(- yawing + goalYaw);
  //int rolling = event.orientation.x;
   wristIK(newYaw,newPitch);

  

//   if (switchEMGPressed%2){
//    ForearmMovement();
//  //   Serial.println("Forearm movement active");
//   } else {
//    HandMovement();
//   //  Serial.println("Hand movement active");
//   }

  //Serial.print(EMGvalues[0]); Serial.print(", "); Serial.println(EMGvalues[1]); 
 
  timeStamp = micros() - timeStamp;
  //if (TIMING_DEBUG) {
    // Serial.print(average[0]); Serial.print(", "); Serial.println(average[1]);
    //can output stuff here for debugging
  //}
  timeCount++;
  delayMicroseconds(500);
  // if more than timeBudget, the sample rate need to reduce to
  // SAMPLE_FREQ_500HZ
}



// }
// revised?
void WristMovement() {

  jxval = analogRead(joyWristPin[0]); // read the x value of the joystick
  jxincr = (jxval>490 && jxval<510) ? 0 : map(jxval, 0, 1023, -2, 2); // map x to acceptable values
  jyval = analogRead(joyWristPin[1]); // read the y value of the joystick
  jyincr= (jyval>500 && jyval<520) ? 0: map(jyval, 0, 1023, -2, 2); // map x to acceptable values;
  //jyincr = map(jyval, 0, 1023, -2, 2); // map y to acceptable values
  // x (LR) movement
    wristLAng=wristLAng-jxincr;
    wristRAng=wristRAng+jxincr;

  // y (UD) movement

    wristLAng=wristLAng+jyincr;
    wristRAng=wristRAng+jyincr;
    

  wristLAng=(wristLAng<130) ? wristLAng : 130;
  wristLAng=(wristLAng>50) ? wristLAng : 50;
  
  wristRAng=(wristRAng<130) ? wristRAng : 130;
  wristRAng=(wristRAng>50) ? wristRAng : 50;

  WristLAct.write(wristLAng); // if j val is left
  WristRAct.write(wristRAng);

}

void HandMovement() {  //Hand Movement
  if (average[1] > (average[0]+EMG_THRESHOLD) && actHandAng >= 21) {
    actHandAng--;
  } else if (average[0] > (average[1]+EMG_THRESHOLD) && actHandAng <= 179) {
    actHandAng++;
  }
  HandIMAct.write(actHandAng);
  HandRPAct.write(map(actHandAng,20,180,180,20)); //opposite bc the servo goes the other way
}

void ForearmMovement() { //moves the forarm 90 when button pressed
  if (average[1] > (average[0]+EMG_THRESHOLD) && actForearmAng >= 11) {
    actForearmAng--;
  } else if (average[0] > (average[1]+EMG_THRESHOLD) && actForearmAng <= 169) {
    actForearmAng++;
  }
  ForearmAct.write(actForearmAng);
  }

int readSensors() {
  for (int n = 0; n < NumOfSensors; n++) {
    EMGvalues[n] = analogRead(SensorInputPins[n]);
    EMGvalues[n] = EMGs[n].update(EMGvalues[n]);
    int envlope = sq(EMGvalues[n]);
    EMGvalues[n] = (envlope > SensorThresholds[n]) ? envlope : 0;
  }
}

int averaging() {
  for (int n = 0; n < NumOfSensors; n++) {
    sum[n] = sum[n] + EMGvalues[n];
    if (count == sampl) {
      average[n] = sum[n] / sampl;
      if(average[n]<0) {
        average[n] = 0;
      }
      sum[n] = 0;
      if(n==NumOfSensors-1) {
        count = 0;
      }
    }
  }
  count++;
}

void constructionSetup(){
  wristL.write(90);
  wristR.write(90);
}

void wristIK(int pitchIK,int yawIK){
  leftAngle=-pitchIK+yawIK+90;
  leftAngle=max(0,min(leftAngle,180));
  rightAngle=(pitchIK+yawIK)+90;
  rightAngle=max(0,min(rightAngle,180));
  wristL.write(leftAngle);
  wristR.write(rightAngle);
  Serial.print(" | leftAngle: ");
  Serial.print(leftAngle);
  Serial.print(" | rightAngle: ");
  Serial.print(rightAngle);
  Serial.print(" | pitch: ");
  Serial.print(pitchIK);
  Serial.print(" | yaw: ");
  Serial.println(yawIK);

   
  

  
  
}


