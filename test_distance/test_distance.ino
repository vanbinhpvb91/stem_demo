const int trigPin = 2;
const int echoPin = 3;

void setup() {
  Serial.begin(9600);
}

void loop()
{
  long duration, inches;
  float cm;
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  cm = microsecondsToCentimeters(duration);
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  delay(100);
}


float microsecondsToCentimeters(long microseconds)

{return microseconds * 1.0f / 29 * 1.0f / 2 * 1.0f;}
