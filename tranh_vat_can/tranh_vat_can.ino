#include <Servo.h> 

int servoPin = 6;
Servo Servo1; 
const int trigPin = 2;
const int echoPin = 3;
int* distances = new int[2];

// chân điều khiển động cơ
int motorPins[] = {8, 9, 12, 13};

void setup() {
  Serial.begin(9600);
  // khởi tại chân điều khiển động cơ xuất tín hiệu
  for (int i = 0; i < 4; i++) {
    pinMode(motorPins[i], OUTPUT);
  }

  Servo1.attach(servoPin);
  Servo1.write(90);
  delay(100);
  Servo1.detach();
}

void loop(){
  int distance = getDistance();
  Serial.println(distance);
  if (distance <= 8) {
    back();
    delay(500);
    stopMotor();
    getDistanceMap();
    if (distances[0] > distances[1]) {
      turnLeft();
      delay(30);
    } else {
      turnRight();
      delay(30);
    }
    Servo1.write(90);
    delay(500);
  }
  else {
    go();
  }
}

void getDistanceMap() {
  Servo1.attach(servoPin);
  
  Servo1.write(0); 
  delay(1000);
  distances[0] = getDistance();
  
  Servo1.write(180); 
  delay(1000);
  distances[1] = getDistance();

  Serial.print(distances[0]);
  Serial.print(" - ");
  Serial.print(distances[1]);
  Serial.println();

  Servo1.write(90); 
  delay(100);
  Servo1.detach();
}

// lệnh dừng động cơ
void stopMotor() {
  rotate(0, 0, 0);
  rotate(1, 0, 0);
}

// lệnh rẽ trái
void turnLeft() {
  rotate(0, 1, 250);
  rotate(1, 0, 150);
}

// lệnh rẽ phải
void turnRight() {
  rotate(0, 0, 150);
  rotate(1, 1, 250);
}

// tiến về phía trước
void go() {
  rotate(0, 1, 255);
  rotate(1, 1, 255);
}

// tiến về phía trước
void back() {
  rotate(0, 0, 180);
  rotate(1, 0, 180);
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

int getDistance() {
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  pinMode(echoPin, INPUT);
  long duration = pulseIn(echoPin, HIGH);
  return duration / 29 / 2;
}
