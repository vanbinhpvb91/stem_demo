#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#define SERVOMIN  125 
#define SERVOMAX  575

#define VX_PIN 0
#define VY_PIN 1

#define MID_ANALOG 488
#define DRAG_PIN 2

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

uint8_t servonum = 0;
int yPulse = 0;

int servoMaxArray[] = {255, 255, 345};
int xPulseArray[] = {SERVOMIN, SERVOMIN, SERVOMIN};

void setup() {
  Serial.begin(9600);
  Serial.println("16 channel Servo test!");

  pwm.begin();
  
  pwm.setPWMFreq(60);

  delay(100);
  pwm.setPWM(0, 0, (SERVOMIN + SERVOMAX)/2);
  pwm.setPWM(1, 0, xPulseArray[0]);
  pwm.setPWM(2, 0, xPulseArray[1]);
  pwm.setPWM(3, 0, xPulseArray[2]);
  delay(1000);
  yPulse = (SERVOMIN + SERVOMAX)/2;

  pinMode(DRAG_PIN, INPUT_PULLUP);
  pinMode(13, OUTPUT);
}

bool preInput = true;
bool isOpen = false;

void loop() {
  //dragAnDrop();
  changeXPosition();
  changeYPosition();

  //pwm.setPWM(4, 0, 200);
  delay(100);

  //pwm.setPWM(4, 0, 600);
  //delay(1000);

  //pwm.setPWM(4, 0, 200);
  //delay(100);
  bool nowInput = digitalRead(DRAG_PIN);
  if (preInput && !nowInput) {
    Serial.print("Click");
    dragAndDrop();
  }
  preInput = nowInput;
}

void changeYPosition() {
  int ySignal = analogRead(VY_PIN);
  int changePulse = calculatePulseChange(ySignal);
  if (ySignal < MID_ANALOG) {
    yPulse -= changePulse;
    yPulse = max(yPulse, SERVOMIN);
  }
  else
  {
    yPulse += changePulse;
    yPulse = min(yPulse, SERVOMAX);
  }
  pwm.setPWM(0, 0, yPulse);
}

void changeXPosition() {
  int xSignal = analogRead(VX_PIN);
  int changePulse = calculatePulseChange(xSignal);

  if (xSignal < MID_ANALOG) {
    if (xPulseArray[2] <= SERVOMIN) {
      xPulseArray[2] = SERVOMIN;
      if (xPulseArray[1] <= SERVOMIN) {
        xPulseArray[1] = SERVOMIN;
        if (xPulseArray[0] <= SERVOMIN) {
          xPulseArray[0] = SERVOMIN;
        }
        else {
          xPulseArray[0] -= changePulse;
        }
      } else {
        xPulseArray[1] -= changePulse;
      }
    } else {
      xPulseArray[2] -= changePulse;
    }
  }

  if (xSignal > MID_ANALOG) {
    if (xPulseArray[0] >= servoMaxArray[0]) {
      xPulseArray[0] = servoMaxArray[0];
      if (xPulseArray[1] >= servoMaxArray[1]) {
        xPulseArray[1] = servoMaxArray[1];
        if (xPulseArray[2] >= servoMaxArray[2]) {
          xPulseArray[2] = servoMaxArray[2];
        }
        else {
          xPulseArray[2] += changePulse;
        }
      } else {
        xPulseArray[1] += changePulse;
      }
    } else {
      xPulseArray[0] += changePulse;
    }
  }
  
  Serial.print(xPulseArray[0]);
  Serial.print(" - ");
  Serial.print(xPulseArray[1]);
  Serial.print(" - ");
  Serial.println(xPulseArray[2]);
  
  pwm.setPWM(1, 0, xPulseArray[0]);
  pwm.setPWM(2, 0, xPulseArray[1]);
  pwm.setPWM(3, 0, xPulseArray[2]);
}

void dragAndDrop() {
  int pulse = isOpen ? 600 : 200;
  pwm.setPWM(5, 0, pulse);
  delay(1000);
  pwm.setPWM(5, 0, 0);
  isOpen = !isOpen;
}

int calculatePulseChange(int pSignal) {
  return map(abs(pSignal - MID_ANALOG),0, MID_ANALOG, 0, 20);
}
