 import the servo library
#include Servo.h

Servo actuator;
int potpin = 0;
int pvalue;
int actang;

void setup() 
{
  actuator.attach(9);
 Serial.begin(9600);
}

void loop()
{
  pvalue = analogRead(potpin);
  actang = map(pvalue, 0, 1023, 1000, 2000);
  Serial.println(Pot angle);
  Serial.println(pvalue);
  actuator.writeMicroseconds(actang);
}