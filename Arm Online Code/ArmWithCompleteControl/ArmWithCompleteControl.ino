//import the servo library
#include "Servo.h"
#include <EMGFilters.h>

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

const int handRPPin=5;
const int handIMPin=6;
const int forearmActPin=11;
const int switchEMGButton=4;
const int EMG_THRESHOLD=20;

static byte SensorInputPins[] = { A2, A3};
static int SensorThresholds[] = { 203, 81}; //Nia 36 and 20

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
int buttonValue;
int buttonFlag = 0;

EMGFilters EMGs[NumOfSensors];
int EMGvalues[NumOfSensors];

double average[NumOfSensors];
int sum[NumOfSensors];
int wristRAng=85; int wristLAng=85;
int jxincr=0; int jyincr=0;

unsigned long timeStamp;
unsigned long timeBudget;

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
  

  for (int n = 0; n < NumOfSensors; n++) { // Setup for the sensors
    EMGs[n].init(sampleRate, humFreq, true, true, true);
  }
  Serial.begin(9600);
  timeBudget = 1e6 / sampleRate;
  pinMode(switchEMGButton, INPUT_PULLUP);
  delay(50);  
}

void loop() {
  timeStamp = micros();  
  readSensors();
  averaging();
  WristMovement();
  
  buttonValue = !digitalRead(switchEMGButton);

  if (buttonValue == 1 && buttonFlag == 0 && millis() > time + 250) {
    switchEMGPressed++;
    time=millis();
    buttonFlag = 1;
  } else if (buttonValue == 0 && buttonFlag == 1) {
    buttonFlag = 0;
  }

  
  if (switchEMGPressed%2){
   ForearmMovement();
 //   Serial.println("Forearm movement active");
  } else {
  // HandMovement();
  //  Serial.println("Hand movement active");
  }

  Serial.print(EMGvalues[0]); Serial.print(", "); Serial.println(EMGvalues[1]); 
 
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