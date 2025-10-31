#define index1 9
#define index2 8
#define middle1 6
#define middle2 7
#define ring1 4 
#define ring2 5
#define pinky1 3
#define pinky2 2
#define thumb1 19 //temporary
#define thumb2 18 //temporary
#define indexPot 0
#define middlePot 1 
#define ringPot 2
#define pinkyPot 3
#define thumbPot 6
#define PWM 10
#define Standby 21

#define FULLEXTENTION 550

int PotVal1,PotVal2,PotVal3,PotVal4;
String p1=",";

int counter,tempPos,currentPos;
int pinkyStopPoint,indexStopPoint,middleStopPoint,ringStopPoint,fingerStopPoint;
String message;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600);

  pinMode(index1, OUTPUT);
  pinMode(index2, OUTPUT);
  pinMode(middle1, OUTPUT);
  pinMode(middle2, OUTPUT);
  pinMode(ring1, OUTPUT);
  pinMode(ring2, OUTPUT);
  pinMode(pinky1, OUTPUT);
  pinMode(pinky2, OUTPUT);
  pinMode(thumb1, OUTPUT);
  pinMode(thumb2, OUTPUT);
  pinMode(indexPot, INPUT);
  pinMode(middlePot, INPUT);
  pinMode(ringPot, INPUT);
  pinMode(pinkyPot, INPUT);
  pinMode(thumbPot, INPUT);

  pinMode(PWM, OUTPUT);
  pinMode(Standby, OUTPUT);

  analogWrite(PWM , 256);
  digitalWrite(Standby, 1);

  /*
  indexStopPoint = initialiseFinger(index1,index2,indexPot,1);
  middleStopPoint = initialiseFinger(middle1,middle2,middlePot,1);
  ringStopPoint = initialiseFinger(ring1,ring2,ringPot,0);
  pinkyStopPoint = initialiseFinger(pinky1,pinky2,pinkyPot,0);
  Serial.println(indexStopPoint); Serial.println(middleStopPoint); Serial.println(ringStopPoint); Serial.println(pinkyStopPoint);*/
  indexStopPoint=504;
  middleStopPoint=504;
  ringStopPoint=504;
  pinkyStopPoint=504;
}

void loop() {
  // put your main code here, to run repeatedly:
  message=getSerial();
  Serial1.println(message);
  if (message=="Open"){
    openAllFingers;
  }
  else if (message=="Close"){
    closeAllFingers;
  }
}


int initialiseFinger(int pin1, int pin2, int potPin, bool direction){
  fingerStopPoint = closeFingerToStop(pin1, pin2, potPin);
  openFingerFull(pin1, pin2, potPin, fingerStopPoint, direction);
  return fingerStopPoint;
}

void closeAllFingers(){
  closeFinger(pinky1, pinky2);
  closeFinger(index1, index2);
  closeFinger(middle1, middle2);
  closeFinger(ring1, ring2);
}

void openAllFingers(){
  openFinger(pinky1, pinky2);
  openFinger(index1, index2);
  openFinger(middle1, middle2);
  openFinger(ring1, ring2);
  counter = 0;
  while(counter<4){
    if(analogRead(indexPot) != (((indexStopPoint+FULLEXTENTION) % 1024))){
      stopFinger(index1,index2);
      counter++;
    }else if (analogRead(middlePot) != (((middleStopPoint+FULLEXTENTION) % 1024))){
      stopFinger(middle1,middle2);
      counter++;
    }else if (analogRead(ringPot) != (((ringStopPoint+FULLEXTENTION) % 1024))){
      stopFinger(ring1,ring2);
      counter++;
    }else if (analogRead(pinkyPot) != (((pinkyStopPoint+FULLEXTENTION) % 1024))){
      stopFinger(pinky1,pinky2);
      counter++;
    }
    }

}

int closeFingerToStop(int H1, int H2, int PotPin){
  counter = 0;
  closeFinger(H1,H2);

  tempPos = analogRead(PotPin);
  do{
    delay(10);
    currentPos = analogRead(PotPin);
    if(currentPos >= tempPos-4 && currentPos <= tempPos+4){
      counter++;
    }else{
      counter = 0;
      tempPos = currentPos;
    }
  }while (counter < 50);

  stopFinger(H1,H2);
  return currentPos;
}

void openFingerFull(int H1, int H2, int PotPin, int fingerStopPoint, int flipLogic){
  openFinger(H1,H2);
  if(flipLogic){
    while(analogRead(PotPin) != (((fingerStopPoint+FULLEXTENTION) % 1024))){};
  }else{
    while(analogRead(PotPin) != (((fingerStopPoint+FULLEXTENTION) % 1024))){};
  }
  stopFinger(H1,H2);
}

void closeFinger(int H1, int H2){
  digitalWrite(H1, 1);
  digitalWrite(H2, 0);
}

void openFinger(int H1, int H2){
  digitalWrite(H1, 0);
  digitalWrite(H2, 1);
}

void stopFinger(int H1, int H2){
  digitalWrite(H1, 0);
  digitalWrite(H2, 0);
}

String getSerial(){  //gets the serial by prompting with the above prompt and then manages it
  while (!(Serial1.available()>0));  //waits for a response (NOTE: this is only called when the system knows for sure its getting a response)
  message=Serial1.readStringUntil('\n');
  message.trim();
  return message;
}