//Ubi1
int motor_pin_11 = 2;
int motor_pin_12 = 3;
int motor_pin_13 = 4;
int motor_pin_14 = 5;
//Ubi2
int motor_pin_21 = 6;
int motor_pin_22 = 7;
int motor_pin_23 = 8;
int motor_pin_24 = 9;
//Ubi3
int motor_pin_31 = 10;
int motor_pin_32 = 11;
int motor_pin_33 = 12;
int motor_pin_34 = 13;
//Ubi4
int motor_pin_41 = 14;
int motor_pin_42 = 15;
int motor_pin_43 = 16;
int motor_pin_44 = 17;

int stepCount = 0;         // number of steps the motor has taken
int turning = 40;
int moving = 0;
bool fr = true;

void setup() {
  Serial.begin(9600);

  pinMode(motor_pin_11, OUTPUT);
  pinMode(motor_pin_12, OUTPUT);
  pinMode(motor_pin_13, OUTPUT);
  pinMode(motor_pin_14, OUTPUT);

  pinMode(motor_pin_21, OUTPUT);
  pinMode(motor_pin_22, OUTPUT);
  pinMode(motor_pin_23, OUTPUT);
  pinMode(motor_pin_24, OUTPUT);

  pinMode(motor_pin_31, OUTPUT);
  pinMode(motor_pin_32, OUTPUT);
  pinMode(motor_pin_33, OUTPUT);
  pinMode(motor_pin_34, OUTPUT);

  pinMode(motor_pin_41, OUTPUT);
  pinMode(motor_pin_42, OUTPUT);
  pinMode(motor_pin_43, OUTPUT);
  pinMode(motor_pin_44, OUTPUT);
}

void loop() {
  if(Serial.available() > 0) {
    moving = Serial.read();
  }

  if(moving) {
    if(stepCount == turning) {
      fr = false;
      stepCount = turning-2;
    } else if(stepCount == -1){
      fr = true;
      stepCount = 0;
      moving = 0;
    }
  
    stepMotor(1, stepCount%4);
    delay(4);
  
    if (fr) stepCount++;
    else stepCount--;
  }
}

void stepMotor(int ubi, int thisStep)
{
  int stepMotorPin1, stepMotorPin2, stepMotorPin3, stepMotorPin4;

  //which ubi to move
  switch (ubi) {
  case 1:
    stepMotorPin1 = motor_pin_11;
    stepMotorPin2 = motor_pin_12;
    stepMotorPin3 = motor_pin_13;
    stepMotorPin4 = motor_pin_14;
    break;
  case 2:
    stepMotorPin1 = motor_pin_21;
    stepMotorPin2 = motor_pin_22;
    stepMotorPin3 = motor_pin_23;
    stepMotorPin4 = motor_pin_24;
    break;
  case 3:
    stepMotorPin1 = motor_pin_31;
    stepMotorPin2 = motor_pin_32;
    stepMotorPin3 = motor_pin_33;
    stepMotorPin4 = motor_pin_34;
    break;
  case 4:
    stepMotorPin1 = motor_pin_41;
    stepMotorPin2 = motor_pin_42;
    stepMotorPin3 = motor_pin_43;
    stepMotorPin4 = motor_pin_44;
    break;

  default:
    return;
  } 

  switch (thisStep) {
  case 0:    // 1010
    digitalWrite(stepMotorPin1, HIGH);
    digitalWrite(stepMotorPin2, LOW);
    digitalWrite(stepMotorPin3, HIGH);
    digitalWrite(stepMotorPin4, LOW);
    break;
  case 1:    // 0110
    digitalWrite(stepMotorPin1, LOW);
    digitalWrite(stepMotorPin2, HIGH);
    digitalWrite(stepMotorPin3, HIGH);
    digitalWrite(stepMotorPin4, LOW);
    break;
  case 2:    //0101
    digitalWrite(stepMotorPin1, LOW);
    digitalWrite(stepMotorPin2, HIGH);
    digitalWrite(stepMotorPin3, LOW);
    digitalWrite(stepMotorPin4, HIGH);
    break;
  case 3:    //1001
    digitalWrite(stepMotorPin1, HIGH);
    digitalWrite(stepMotorPin2, LOW);
    digitalWrite(stepMotorPin3, LOW);
    digitalWrite(stepMotorPin4, HIGH);
    break;
  } 
}

