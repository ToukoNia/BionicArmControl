#include <Servo.h>
const byte servoPin[2] = {4,5};
//const byte PLPot = 0;
//const byte PRPot = 1;

Servo wristL;
Servo wristR;

int wristLAng=1500;
int wristRAng=1500;
byte jxincr; byte jyincr;
int jxval; int jyval;

const byte joyWristPin[2] = {0, 1};
void setup() {
  // put your setup code here, to run once:
  wristL.attach(servoPin[0]);
  wristR.attach(servoPin[1]);
  Serial.begin(9600);

}

void loop() {

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

    wristL.writeMicroseconds(wristLAng);
    wristR.writeMicroseconds(wristRAng);
 // wristL.write(map(analogRead(PLPot),0,1023,0, 180));
 // wristR.write(map(analogRead(PRPot),0,1023,0, 180));
    delay(100);
}
