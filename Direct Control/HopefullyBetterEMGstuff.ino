#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "Servo.h"
#include "EMGFilters.h"

#define TIMING_DEBUG 1

// emg filter only support "SAMPLE_FREQ_500HZ" or "SAMPLE_FREQ_1000HZ"
int sampleRate = SAMPLE_FREQ_500HZ;
// emg filter only support "NOTCH_FREQ_50HZ" or "NOTCH_FREQ_60HZ"
int humFreq = NOTCH_FREQ_50HZ;

#define NumOfSensors 4
static byte SensorInputPins[] = { A0, A1, A2, A3 };
static int SensorThresholds[] = { 64, 81, 144, 87 };
EMGFilters EMGs[NumOfSensors];
int EMGvalues[NumOfSensors];

#define NumOfServos 3
static int ServoPins[] = { 7, 8, 9 };
Servo Servos[NumOfServos];
//Servo position
int pos[] = { 90, 90, 90 };

// Define the pins for the buttons
#define buttonUpPin  2    // Pin connected to the button for going up
#define buttonDownPin  3  // Pin connected to the button for going down

// Define the total number of switch cases
#define numSwitchCases  3

double average[NumOfSensors];
int sum[NumOfSensors];

//Roughly how many samples in 3 seconds
static int testLen = 1111;
int timeCount = 0;

//How many sampkes it takes for averaging
int sampl = 10;
int count;

unsigned long timeStamp;
unsigned long timeBudget;

int currentCase = 2;

void setup() {
  for (int n = 0; n < NumOfServos; n++) { // Setup for the servos
    Servos[n].attach(ServoPins[n]);
  }
  for (int n = 0; n < NumOfSensors; n++) { // Setup for the sensors
    EMGs[n].init(sampleRate, humFreq, true, true, true);
  }

  // Initialize the button pins as inputs
  pinMode(buttonUpPin, INPUT_PULLUP);
  pinMode(buttonDownPin, INPUT_PULLUP);

  // open serial
  Serial.begin(115200);

  timeBudget = 1e6 / sampleRate;
}

void loop() {
  timeStamp = micros();

  /*------------sensor shenanigans-------------------*/
  readSensors();
  averaging();

  /*------------normal shenanigans-------------------*/
  // Check if the button for going up is pressed
  if (digitalRead(buttonUpPin) == LOW) {
    // Move to the next case
    currentCase++;
    // If current case exceeds the maximum, loop back to the first case
    if (currentCase >= numSwitchCases) {
      currentCase = 0;
    }
  }
  // Check if the button for going down is pressed
  else if (digitalRead(buttonDownPin) == LOW) {
    // Move to the previous case
    currentCase--;
    // If current case is less than 0, loop back to the last case
    if (currentCase < 0) {
      currentCase = numSwitchCases - 1;
    }
  }

  if (average[2] >= 1000 && average[3] >= 1000 && pos[currentCase] <= 180) {
    pronation(currentCase, 1);
  } else if (average[2] >= 2000 && average[3] >= 2000 && pos[currentCase] <= 180) {
    pronation(currentCase, 2);
  }

  if (average[1] >= 1000 && average[4] >= 1000 && pos[currentCase] >= 0) {
    supination(currentCase, 1);
  } else if (average[1] >= 2000 && average[4] >= 2000 && pos[currentCase] >= 0) {
    supination(currentCase, 2);
  }


  timeStamp = micros() - timeStamp;
  if (TIMING_DEBUG) {
    //can output stuff here for debugging
  }
  timeCount++;
  delayMicroseconds(500);
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
      sum[n] = 0;
      count = 0;
    }
  }
  count++;
}

void pronation(int ServoNum, int speed) {
  Servos[ServoNum].write(pos[ServoNum] += speed);
}

void supination(int ServoNum, int speed) {
  Servos[ServoNum].write(pos[ServoNum] -= speed);
}