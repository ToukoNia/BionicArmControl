#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "EMGFilters.h"

#define TIMING_DEBUG 1

#define SensorInputPin1 A0 // input pin number
#define SensorInputPin2 A1 // input pin number

#define SensorInputPin3 A2 // input pin number
#define SensorInputPin4 A3 // input pin number

String motion[7] = {"Elbow flex","Elbow extension","Open hand","Close hand","Supinate","Pronate","Rest"};
int randMotion = 0;
bool needRest = true;


EMGFilters myFilter;
EMGFilters myFilter1;
EMGFilters myFilter2;
EMGFilters myFilter3;
// discrete filters must works with fixed sample frequence
// our emg filter only support "SAMPLE_FREQ_500HZ" or "SAMPLE_FREQ_1000HZ"
// other sampleRate inputs will bypass all the EMG_FILTER
int sampleRate = SAMPLE_FREQ_500HZ;
// For countries where power transmission is at 50 Hz
// For countries where power transmission is at 60 Hz, need to change to
// "NOTCH_FREQ_60HZ"
// our emg filter only support 50Hz and 60Hz input
// other inputs will bypass all the EMG_FILTER
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

unsigned long timeStamp;
unsigned long timeBudget;

void setup() {
    /* add setup code here */
    myFilter.init(sampleRate, humFreq, true, true, true);
    myFilter1.init(sampleRate, humFreq, true, true, true);
    myFilter2.init(sampleRate, humFreq, true, true, true);
    myFilter3.init(sampleRate, humFreq, true, true, true);
    // open serial
    Serial.begin(115200);

    // setup for time cost measure
    // using micros()
    timeBudget = 1e6 / sampleRate;
    // micros will overflow and auto return to zero every 70 minutes
}

void loop() {
    /* add main program code here */
    // In order to make sure the ADC sample frequence on arduino,
    // the time cost should be measured each loop
    /*------------start here-------------------*/
    timeStamp = micros();
    
    //Make sure that the range of rand is the same as length of motion array
    if(timeCount >= testLen){
      timeCount = 0;
      if(needRest == false){
        randMotion = random(5);
        needRest=true; 
      }else if(needRest){
        randMotion = 6;
        needRest=false;        
      }
    }

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

    int envlope2 = sq(DataAfterFilter2);
    // any value under threshold will be set to zero
    envlope2 = (envlope2 > Threshold2) ? envlope2 : 0;

    int envlope3 = sq(DataAfterFilter3);
    // any value under threshold will be set to zero
    envlope3 = (envlope3 > Threshold3) ? envlope3 : 0;

    int envlope4 = sq(DataAfterFilter4);
    // any value under threshold will be set to zero
    envlope4 = (envlope4 > Threshold4) ? envlope4 : 0;

    timeStamp = micros() - timeStamp;
    if (TIMING_DEBUG) {
        // Serial.print("Read Data: "); Serial.println(Value);
        // Serial.print("Filtered Data: ");Serial.println(DataAfterFilter);
        //Serial.print("Squared Data: ");
        // Bicep                                    Tricep
        Serial.print(motion[randMotion]); Serial.print(", "); Serial.print(envlope1); Serial.print(", "); Serial.print(envlope2); Serial.print(", "); Serial.print(envlope3); Serial.print(", "); Serial.println(envlope4);
        //Serial.print("Filters cost time: "); Serial.println(timeStamp); 
        // the filter cost average around 520 us
    }
    timeCount++;
    delayMicroseconds(500);
    // if more than timeBudget, the sample rate need to reduce to
    // SAMPLE_FREQ_500HZ
}
