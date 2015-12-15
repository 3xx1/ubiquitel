//Ubi1
const int inA = 2;
const int inB = 3;
const int PS = 4;
const int Vs2B = 5;
const int LED = 13;

int turning = 5;
int dt = 5000;
int stepCount = 0;         // number of steps the motor has taken
int moving = 1;
int fr = 1;

void setup() {
  Serial.begin(9600);

  pinMode(inA, OUTPUT);
  pinMode(inB, OUTPUT);
  pinMode(Vs2B, OUTPUT);
  pinMode(PS, OUTPUT);
  pinMode(LED, OUTPUT);

  digitalWrite(inA, LOW);
  digitalWrite(inB, LOW);
  digitalWrite(Vs2B, LOW);
  digitalWrite(PS, HIGH);
  digitalWrite(LED, LOW);
}

void loop() {
  if(Serial.available() > 0) {
    int value = Serial.read();
    //if(value > 100) dt = (value - 100)*200;
    //else turning = value;
    moving = 1;
  }

  if(moving) {
    stepMotor();
    delayMicroseconds(dt);
    stepCount++;// = stepCount+fr;
    if(stepCount == turning) {
      moving = 0;
      stepCount = 0;
      turning++;
      if(turning == 40) turning = 5;
    }
  }else if(!moving) {
    delay(20);
    stopMotor();
    delay(80);
    moving = 1;
  }
}

void stepMotor()
{
  int thisStep = stepCount%4;
  digitalWrite(Vs2B, HIGH);
  digitalWrite(PS, LOW);
  digitalWrite(LED, HIGH);
  
  switch (thisStep) {
  case 0:    // 00
    digitalWrite(inA, LOW);
    digitalWrite(inB, LOW);
    break;
  case 1:    // 10
    digitalWrite(inA, HIGH);
    digitalWrite(inB, LOW);
    break;
  case 2:    //11
    digitalWrite(inA, HIGH);
    digitalWrite(inB, HIGH);
    break;
  case 3:    //01
    digitalWrite(inA, LOW);
    digitalWrite(inB, HIGH);
    break;
  }
}

void stopMotor() {
  digitalWrite(PS, HIGH);
  digitalWrite(Vs2B, LOW);
  digitalWrite(LED, LOW);
}

