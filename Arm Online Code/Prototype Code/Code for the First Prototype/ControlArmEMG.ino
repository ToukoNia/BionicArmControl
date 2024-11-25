
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
const int potWristPin = 0;
const int potHandPin = 1;
const int buttonWristPin=3;
const int wristLActPin=8;
const int wristRActPin=9;
const int handRPPin=5;
const int handIMPin=6;
const int forearmActPin=11;
const int forearmButton=4;
static byte SensorInputPins[] = { A2, A3};
static int SensorThresholds[] = { 204, 81};

// emg filter only support "SAMPLE_FREQ_500HZ" or "SAMPLE_FREQ_1000HZ"
int sampleRate = SAMPLE_FREQ_500HZ;
// emg filter only support "NOTCH_FREQ_50HZ" or "NOTCH_FREQ_60HZ"
int humFreq = NOTCH_FREQ_50HZ;

#define TIMING_DEBUG 1
#define NumOfSensors 2

//values needed for calculations
int buttonForearmPressed=0;
int pvalue;
int actang;
int buttonWristPressed=0;
int time=0;
int wristTime=0;
int timeCount = 0;

int actHandAng=20;

#define sampl 20
int count;

EMGFilters EMGs[NumOfSensors];
int EMGvalues[NumOfSensors];

double average[NumOfSensors];
int sum[NumOfSensors];



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
  delay(50);  
}

void loop(){
  timeStamp = micros();  
  readSensors();
  averaging();
   HandMovement();
  WristMovement();
  ForearmMovement();

  timeStamp = micros() - timeStamp;
  if (TIMING_DEBUG) {
    Serial.print(average[0]); Serial.print(", "); Serial.println(average[1]);
    //can output stuff here for debugging
  }
  timeCount++;
  delayMicroseconds(500);
  // if more than timeBudget, the sample rate need to reduce to
  // SAMPLE_FREQ_500HZ
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
  if (average[1] > (ave
rage[0]+20) && actHandAng >= 20) {
    actHandAng=actHandAng-2;
  } else if(average[0] > (average[1]+20) && actHandAng <= 180) {
    actHandAng=actHandAng+2;
  }
  HandIMAct.write(actHandAng);
  HandRPAct.write(map(actHandAng,20,180,180,20)); //opposite bc the servo goes the other way
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
      if(average[n]<0){
        average[n]=0;
      }
      sum[n] = 0;
      if(n==NumOfSensors-1){
        count = 0;
      }
    }
  }
  count++;
}
