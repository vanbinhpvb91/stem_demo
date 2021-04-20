#include <SoftwareSerial.h>

int rxPin = 3;
int txPin = 2;
SoftwareSerial hc06(rxPin, txPin);

String message;

// chân điều khiển động cơ
int motorPins[] = {5, 6, 9, 10};

byte Speed = 255;

void setup()
{
  Serial.begin(9600);
  // khởi tại chân điều khiển động cơ xuất tín hiệu
  for (int i = 0; i < 4; i++) {
    pinMode(motorPins[i], OUTPUT);
  }
  hc06.begin(9600); // đặt baudrate giao tiếp hc06
  delay(2000);
}

void loop() {
  if(hc06.available())
  {
    char ch = hc06.read();
    Serial.println(ch);
    
    if (ch == 'F') {
      go();
    }
    if (ch == 'B') {
      back();
    }
    if (ch == 'R') {
      turnAroundRight();
    }
    if (ch == 'L') {
      turnAroundLeft();
    }
    if (ch == 'G') {
      turnLeft();
    }
    if (ch == 'I') {
      turnRight();
    }
    if (ch == 'S') {
      stopMotor();
    }
    
  }
}

// lệnh dừng động cơ
void stopMotor() {
  rotate(0, 0, 0);
  rotate(1, 0, 0);
}

// lệnh rẽ trái
void turnLeft() {
  rotate(0, 1, Speed);
  rotate(1, 1, Speed - 80);
}

// lệnh rẽ phải
void turnRight() {
  rotate(0, 1, Speed - 80);
  rotate(1, 1, Speed);
}

// lệnh xoay vòng bên trái
void turnAroundLeft(){
  rotate(0, 1, Speed);
  rotate(1, 0, Speed);
}

// lệnh xoay vòng bên phải
void turnAroundRight(){
  rotate(0, 0, Speed);
  rotate(1, 1, Speed);
}

// tiến về phía trước
void go() {
  rotate(0, 1, Speed);
  rotate(1, 1, Speed);
}

// tiến về phía trước
void back() {
  rotate(0, 0, Speed);
  rotate(1, 0, Speed);
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
