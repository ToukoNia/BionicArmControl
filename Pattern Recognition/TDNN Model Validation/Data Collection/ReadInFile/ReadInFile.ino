//import the servo library
#include "Servo.h"
#include <EMGFilters.h>


static byte SensorInputPins[] = { A0, A1};
static int SensorThresholds[] = { 0, 0};

String motion[7] = {"Elbow flex","Elbow extension","Open hand","Close hand","Supinate","Pronate","Rest"};
int randMotion = 0;
bool needRest = true;

// emg filter only support "SAMPLE_FREQ_500HZ" or "SAMPLE_FREQ_1000HZ"
int sampleRate = SAMPLE_FREQ_500HZ;
// emg filter only support "NOTCH_FREQ_50HZ" or "NOTCH_FREQ_60HZ"
int humFreq = NOTCH_FREQ_50HZ;

#define TIMING_DEBUG 1
#define NumOfSensors 2


int timeCount = 0;


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
  for (int n = 0; n < NumOfSensors; n++) { // Setup for the sensors
    EMGs[n].init(sampleRate, humFreq, true, true, true);
  }
  Serial.begin(9600);
  timeBudget = 1e6 / sampleRate;
  delay(50);  
}

void loop() {
  timeStamp = micros();  
  readSensors();

  timeStamp = micros() - timeStamp;
  timeCount++;
  delayMicroseconds(500);

  if (TIMING_DEBUG){
    Serial.print(EMGvalues[0]); Serial.print(", "); Serial.println(EMGvalues[1]);   
  }
  // if more than timeBudget, the sample rate need to reduce to
  // SAMPLE_FREQ_500HZ
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