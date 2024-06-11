#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Servo.h>
#include "EMGFilters.h"

#define TIMING_DEBUG 1

#define SensorInputPin1 A0 // input pin number
#define SensorInputPin2 A1 // input pin number

#define SensorInputPin3 A2 // input pin number
#define SensorInputPin4 A3 // input pin number

const int servoPins[] = {7,8,9};
Servo servos[3];

// Define the pins for the buttons
const int buttonUpPin = 2;    // Pin connected to the button for going up
const int buttonDownPin = 3;  // Pin connected to the button for going down

// Define the total number of switch cases
const int numSwitchCases = 3;

// Variable to store the current switch case
int currentCase = 2;

EMGFilters myFilter;
EMGFilters myFilter1;
EMGFilters myFilter2;
EMGFilters myFilter3;
// emg filter only support "SAMPLE_FREQ_500HZ" or "SAMPLE_FREQ_1000HZ"
int sampleRate = SAMPLE_FREQ_500HZ;
// emg filter only support "NOTCH_FREQ_50HZ" or "NOTCH_FREQ_60HZ"
int humFreq = NOTCH_FREQ_50HZ;

// Calibration:
// put on the sensors, and release your muscles;
// wait a few seconds, and select the max value as the threshold;
// any value under threshold will be set to zero
static int Threshold1 = 64;
static int Threshold2 = 81;
static int Threshold3 = 144;
static int Threshold4 = 87;

//Roughly how many samples in 3 seconds
static int testLen = 1111;
int timeCount = 0;

// Stuff for averaging
int count1 = 0;
double average1 = 0;
int sum1 = 0;

int count2 = 0;
double average2 = 0;
int sum2 = 0;

int count3 = 0;
double average3 = 0;
int sum3 = 0;

int count4 = 0;
double average4 = 0;
int sum4 = 0;

//How many sampkes it takes for averaging
int sampl1 = 10;

//Servo position
int pos[] = {90,90,90};

//Servo adjustment in deg per sec
//Change to alter the speed of movment
static int slowMove=1;
static int fastMove=2;

unsigned long timeStamp;
unsigned long timeBudget;

void setup() {
    for(int n = 0; n < 3; n++){
      servos[n].attach(servoPins[n]);
    }
    
    myFilter.init(sampleRate, humFreq, true, true, true);
    myFilter1.init(sampleRate, humFreq, true, true, true);
    myFilter2.init(sampleRate, humFreq, true, true, true);
    myFilter3.init(sampleRate, humFreq, true, true, true);

    // Initialize the button pins as inputs
    pinMode(buttonUpPin, INPUT_PULLUP);
    pinMode(buttonDownPin, INPUT_PULLUP);

    // open serial
    Serial.begin(115200);

    timeBudget = 1e6 / sampleRate;
}

void loop() {
    timeStamp = micros();
    /*------------normal shenanignas-------------------*/
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

    if(average2 >= 1000 && average3 >= 1000 && pos[currentCase]<=180){
      servos[currentCase].write(pos[currentCase]+=slowMove);
    }else if(average2 >= 2000 && average3 >= 2000 && pos[currentCase]<=180){
      servos[currentCase].write(pos[currentCase]+=fastMove);
    }

    if(average1 >= 1000 && average4 >= 1000 && pos[currentCase]>=0){
      servos[currentCase].write(pos[currentCase]-=slowMove);
    }else if(average1 >= 2000 && average4 >= 2000 && pos[currentCase]>=0){
      servos[currentCase].write(pos[currentCase]-=fastMove);
    }
  
    /*------------sensor shenanignas-------------------*/

    int Value1 = analogRead(SensorInputPin1);
    int Value2 = analogRead(SensorInputPin2);
    int Value3 = analogRead(SensorInputPin3);
    int Value4 = analogRead(SensorInputPin4);

    // filter processing
    int DataAfterFilter1 = myFilter.update(Value1);
    int DataAfterFilter2 = myFilter1.update(Value2);
    int DataAfterFilter3 = myFilter2.update(Value3);
    int DataAfterFilter4 = myFilter3.update(Value4);

    int envlope1 = sq(DataAfterFilter1);
    // any value under threshold will be set to zero
    envlope1 = (envlope1 > Threshold1) ? envlope1 : 0;


    //Averaging
    sum1=sum1+envlope1;
    count1++;
    if(count1==sampl1){
      average1=sum1/sampl1;
      sum1=0;
      count1=0;
    }

    int envlope2 = sq(DataAfterFilter2);
    // any value under threshold will be set to zero
    envlope2 = (envlope2 > Threshold2) ? envlope2 : 0;

    //Averaging
    sum2=sum2+envlope2;
    count2++;
    if(count2==sampl1){
      average2=sum2/sampl1;
      sum2=0;
      count2=0;
    }

    int envlope3 = sq(DataAfterFilter3);
    // any value under threshold will be set to zero
    envlope3 = (envlope3 > Threshold3) ? envlope3 : 0;

    //Averaging
    sum3=sum3+envlope3;
    count3++;
    if(count3==sampl1){
      average3=sum3/sampl1;
      sum3=0;
      count3=0;
    }

    int envlope4 = sq(DataAfterFilter4);
    // any value under threshold will be set to zero
    envlope4 = (envlope4 > Threshold4) ? envlope4 : 0;

    //Averaging
    sum4=sum4+envlope4;
    count4++;
    if(count4==sampl1){
      average4=sum4/sampl1;
      sum4=0;
      count4=0;
    }

    timeStamp = micros() - timeStamp;
    if (TIMING_DEBUG) {
        Serial.print(envlope1); Serial.print(", "); Serial.print(envlope2); Serial.print(", "); Serial.print(envlope3); Serial.print(", "); Serial.println(envlope4);
    }
    timeCount++;
    delayMicroseconds(500);
    // if more than timeBudget, the sample rate need to reduce to
    // SAMPLE_FREQ_500HZ
}