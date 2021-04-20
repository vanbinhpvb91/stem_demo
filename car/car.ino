// chân điều khiển động cơ
int motorPins[] = {5, 6, 9, 10};

// chân nhận tín hiệu cảm biến
int irPin[] = {2, 3};

unsigned long distance = 0;

void setup() {
  Serial.begin(9600);
  // khởi tại chân điều khiển động cơ xuất tín hiệu
  for (int i = 0; i < 4; i++) {
    pinMode(motorPins[i], OUTPUT);
  }
  // khởi tạo chân cảm biến nhận tín hiệu
  pinMode(irPin[1], INPUT);
  pinMode(irPin[2], INPUT);
}

bool lastTurn = false;

void loop() {
  // lấy tín hiệu từ cảm biến
  bool rightInput = digitalRead(irPin[1]);
  bool leftInput = digitalRead(irPin[0]);
  

  // nếu xe ra ngoài đường line
  if (!leftInput && !rightInput) {
    if (lastTurn) {
      while (!leftInput && !rightInput) {
        turnLeft();
        delay(100);
        leftInput = digitalRead(irPin[0]);
        rightInput = digitalRead(irPin[1]);
      }
    } else {
      while (!leftInput && !rightInput) {
        turnRight();
        delay(100);
        leftInput = digitalRead(irPin[0]);
        rightInput = digitalRead(irPin[1]);
      }
    }
    return;
  }
  // nếu chân bên trái ra ngoài đường line
  if (!leftInput) {
    turnRight();
    lastTurn = false;
    return;
  }
  // nếu chân bên phải ra ngoài đường line
  if (!rightInput) {
    turnLeft();
    lastTurn = true;
    return;
  }
  // nếu cả 2 chân nằm trong đường line
  if (rightInput && leftInput) {
    go();
    return;
  }
  
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

// lệnh xoay vòng bên trái
void turnAroundLeft(){
  rotate(0, 1, 200);
  rotate(1, 0, 200);
}

// lệnh xoay vòng bên phải
void turnAroundRight(){
  rotate(0, 0, 200);
  rotate(1, 1, 200);
}

// tiến về phía trước
void go() {
  rotate(0, 1, 200);
  rotate(1, 1, 200);
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
