#include "Adafruit_VL53L0X.h"

// chân điều khiển động cơ
int motorPins[] = {8, 9, 10, 11};
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

void setup() {
  Serial.begin(9600);

  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }

  // khởi tại chân điều khiển động cơ xuất tín hiệu
  for (int i = 0; i < 4; i++) {
    pinMode(motorPins[i], OUTPUT);
  }

  Serial.println("Adafruit VL53L0X test");
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  // power 
  Serial.println(F("VL53L0X API Simple Ranging example\n\n")); 
}

float preMilis = -1;

void loop()
{
  int mili = 0;
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false);

  if (measure.RangeStatus != 4) {
    int mili = measure.RangeMilliMeter;
    if (preMilis != -1) {
      turn(100 - mili);
    }
    preMilis = mili;
  } else {
    Serial.println("out");
  }
}

void turn(float minus) {
  int decrease = map(abs(minus), 0.0, 100.0, 0, 200);
  Serial.print(minus);
  Serial.print(" - ");
  Serial.print(decrease);
  decrease = 255 - decrease;
  Serial.print(" - ");
  Serial.println(decrease);
  if(decrease < 0) decrease = 0;
  if (minus > 0) {
    rotate(1, 1, 255);
    rotate(0, 1, decrease);
  }
  else {
    rotate(1, 1, decrease);
    rotate(0, 1, 255);
  }
  delay(200);
}

float microsecondsToMilimeters(long microseconds)
{
  return microseconds * 10.0f / 29 * 1.0f / 2 * 1.0f;
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
