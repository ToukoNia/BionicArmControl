#include <Servo.h>
const byte servoPin[2] = {4,5};
const byte joystickButton = 3;
const byte joyWristPin[2] = {0, 1};
//const byte PLPot = 0;
//const byte PRPot = 1;

Servo wristL;
Servo wristR;

int wristLPulse=1500;
int wristRPulse=1500;
int jxincr; int jyincr;
int jxval; int jyval;
bool buttonFlag=1;
long time=0;
long buttonTime=0;
int joystickButtonPressed=0;
bool buttonValue;
void setup() {
  // put your setup code here, to run once:
  wristL.attach(servoPin[0]);
  wristR.attach(servoPin[1]);
  Serial.begin(9600);
  pinMode(joystickButton, INPUT_PULLUP);

}

void loop() {
  
  buttonValue = !digitalRead(joystickButton);

  if (buttonValue == 1 && buttonFlag == 0 && millis() > buttonTime + 250) {
    joystickButtonPressed++;
    buttonTime=millis();
    buttonFlag = 1;
  } else if (buttonValue == 0 && buttonFlag == 1) {
    buttonFlag = 0;
  }

  
    wristMove();
 // wristL.write(map(analogRead(PLPot),0,1023,0, 180));
 // wristR.write(map(analogRead(PRPot),0,1023,0, 180));


}

void constructionSetup(){
  wristL.write(90);
  wristR.write(90);
}
void wristMove(){
  
   if (!(joystickButtonPressed%2)){
      if (time<millis()-5){
        jxval = analogRead(joyWristPin[0]); // read the x value of the joystick
        jxincr = (jxval>490 && jxval<510) ? 0 : map(jxval, 0, 1023, 5, -5); // map x to acceptable values
        jyval = analogRead(joyWristPin[1]); // read the y value of the joystick
        jyincr= (jyval>500 && jyval<520) ? 0: map(jyval, 0, 1023, -5, 5); // map x to acceptable values;
        //jyincr = map(jyval, 0, 1023, -2, 2); // map y to acceptable values
      // x (LR) movement
        wristLPulse=wristLPulse-jxincr;
        wristRPulse=wristRPulse+jxincr;

      // y (UD) movement

        wristLPulse=wristLPulse+jyincr;
        wristRPulse=wristRPulse+jyincr;

        wristLPulse=max(min(wristLPulse,2000),1000);
        wristRPulse=max(min(wristRPulse,2000),1000);
        time=millis();
      }
        wristL.writeMicroseconds(wristLPulse);
        wristR.writeMicroseconds(wristRPulse);
    //    Serial.print(jyincr); Serial.print(","); Serial.println(jxincr);
    } else {
        wristLPulse=1500;
        wristRPulse=1500;
        wristL.writeMicroseconds(wristLPulse);
        wristR.writeMicroseconds(wristRPulse);
        joystickButtonPressed++;
    }
}
