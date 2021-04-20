int motorPins[] = {6, 9, 10, 11};
int leftCounter = 13;
int rightCounter = 12;

int count = 0;
void setup() {
  for (int i = 0; i < 4; i++) {
    pinMode(motorPins[i], OUTPUT);
  }
  pinMode(leftCounter, INPUT);
  pinMode(rightCounter, INPUT);

  delay(3000);
  
  rotate(0,0,0);
  rotate(1,1,180);

  bool input = digitalRead(rightCounter);

  Serial.begin(9600);
}

bool preInput = 0;

void loop() {
  bool input = digitalRead(rightCounter);
  
  if (preInput != input) {
    count++;
  }
  preInput = input;

  if (count >= 35) {
    rotate(0,0,0);
    rotate(1,0,0);
    Serial.println(count);
  }
}

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
