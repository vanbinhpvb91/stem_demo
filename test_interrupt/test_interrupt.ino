int echoPin = 2; 
int trigPin = 3;
int ledPin = 13;

void setup()
{
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(echoPin), blink, FALLING);
  
  SendTrig();
}

void loop()
{
  
}

unsigned long startTime;

void blink() {
  long dur = micros() - startTime;
  Serial.println(microsecondsToCentimeters(dur));
  delay(300);

  SendTrig();
}

void SendTrig() {
  startTime = micros();
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
}

long microsecondsToCentimeters(long microseconds)
{
  return microseconds / 29 / 2;
}
