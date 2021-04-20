#define SPEED 255
#define R_SPEED 80
#define IN_RANGE 250

// chân điều khiển động cơ
int motorPins[] = {6, 9, 10, 11};
int leftMotorIndex = 0;
int rightMotorIndex = 1;

byte frontPins[] = {2, 3};
byte leftPins[] = {4, 5};
byte rightPins[] = {8,7};

int leftPinCounter = 12;
int rightPinCounter = 13;

void setup() {
  Serial.begin(9600);

  for (int i = 0; i < 4; i++) {
    pinMode(motorPins[i], OUTPUT);
  }
  
  pinMode(leftPinCounter, INPUT);
  pinMode(rightPinCounter, INPUT);

  //changeDir();
  //stopMotor();
}

int leftPreDistance = -1;
int rightPreDistance = -1;

void loop(){
  //return;
  long frontMm = getDistance(frontPins[0], frontPins[1]);
  long leftMm = getDistance(leftPins[0], leftPins[1]);
  long rightMm = getDistance(rightPins[0], rightPins[1]);

  Serial.print("f : ");
  Serial.print(frontMm);
  Serial.print(" - l : ");
  Serial.print(leftMm);
  Serial.print(" - r : ");
  Serial.println(rightMm);

  if (frontMm <= 130 && (leftMm > IN_RANGE || rightMm > IN_RANGE)) {
    changeDir();
    leftPreDistance = -1;
    rightPreDistance = -1;
  } else {
    go(leftMm, rightMm);
  }
}

void go(long leftMm, long rightMm) {
  if (leftMm < IN_RANGE && rightMm < IN_RANGE) {
    if (rightMm < leftMm) {
      rotate(1, 1, SPEED);
      rotate(0, 1, 100);
    } else if (rightMm > leftMm) {
      rotate(1, 1, 100);
      rotate(0, 1, SPEED);
    } else {
      rotate(1, 1, SPEED);
      rotate(0, 1, SPEED);
    }
  }
  else if (leftMm < IN_RANGE) {
    if (leftPreDistance != -1 && leftPreDistance - leftMm > 3) {
      rotate(leftMotorIndex, 1, SPEED);
      rotate(rightMotorIndex, 1, 0);
    } else if (leftPreDistance != -1 && leftPreDistance - leftMm < -3) {
      rotate(leftMotorIndex, 1, 0);
      rotate(rightMotorIndex, 1, SPEED);
    } else {
      rotate(leftMotorIndex, 1, SPEED);
      rotate(rightMotorIndex, 1, SPEED);
    }
    leftPreDistance = leftMm;
  } else if (rightMm < IN_RANGE) {
    if (rightPreDistance != -1 && rightPreDistance - rightMm > 3) {
      rotate(leftMotorIndex, 1, 0);
      rotate(rightMotorIndex, 1, SPEED);
    } else if (rightPreDistance != -1 && rightPreDistance - rightMm < -3) {
      rotate(leftMotorIndex, 1, SPEED);
      rotate(rightMotorIndex, 1, 0);
    } else {
      rotate(leftMotorIndex, 1, SPEED);
      rotate(rightMotorIndex, 1, SPEED);
    }
    rightPreDistance = rightMm;
  } else {
    stopMotor();
  }
  delay(50);
}

void changeDir() {
  Serial.println("------------------begin---------------");
  stopMotor();
  delay(1000);

  int counter = 0;

  long frontMm = getDistance(frontPins[0], frontPins[1]);
  long leftMm = getDistance(leftPins[0], leftPins[1]);
  long rightMm = getDistance(rightPins[0], rightPins[1]);

  Serial.print("f : ");
  Serial.print(frontMm);
  Serial.print(" - l : ");
  Serial.print(leftMm);
  Serial.print(" - r : ");
  Serial.println(rightMm);

  int pinCounter = rightPinCounter;
  bool turnLeft = true;
  if (leftMm > IN_RANGE) {
    rotate(leftMotorIndex, 1, 0);
    rotate(rightMotorIndex, 1, SPEED);
  }
  if (rightMm > IN_RANGE) {
    turnLeft = false;
    rotate(leftMotorIndex, 1, SPEED);
    rotate(rightMotorIndex, 1, 0);
    pinCounter = leftPinCounter;
  }
  
  bool preInput = digitalRead(pinCounter);
  while(counter <= 30) {
    bool input = digitalRead(pinCounter);
    if (preInput != input) {
      counter++;
    }
    preInput = input;
  }

  frontMm = getDistance(frontPins[0], frontPins[1]);
  leftMm = getDistance(leftPins[0], leftPins[1]);
  rightMm = getDistance(rightPins[0], rightPins[1]);
  
  if (turnLeft) {
    rotate(leftMotorIndex, 1, SPEED);
    rotate(rightMotorIndex, 1, 100);
  } else {
    rotate(leftMotorIndex, 1, 100);
    rotate(rightMotorIndex, 1, SPEED);
  }
  delay(200);

  Serial.print("f : ");
  Serial.print(frontMm);
  Serial.print(" - l : ");
  Serial.print(leftMm);
  Serial.print(" - r : ");
  Serial.println(rightMm);
  Serial.println("-------------------------------------");
}

// lệnh dừng động cơ
void stopMotor() {
  rotate(0, 0, 0);
  rotate(1, 0, 0);
}

// lệnh xoay động cơ
// motorIndex là vị trí động cơ
// dir là hướng xoay (tiến hay lùi)
// motorSpeed từ 0 - 255 là tốc độc mong muốn của dộng cơ
void rotate(int motorIndex, bool dir, int motorSpeed) {
  int pulse1 = 0;
  int pulse2 = 0;

  if (dir) {
    pulse2 = motorSpeed;
  } else {
    pulse1 = motorSpeed;
  }

  analogWrite(motorPins[motorIndex * 2], pulse1);
  analogWrite(motorPins[motorIndex * 2 + 1], pulse2);
}

long getDistance(int trigPin, int echoPin) {
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  pinMode(echoPin, INPUT);
  long duration = pulseIn(echoPin, HIGH);
  //delay(20);
  return duration * 10 / 29 / 2;
}
