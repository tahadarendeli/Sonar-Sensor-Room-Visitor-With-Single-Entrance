// defines pins numbers
int pirPin = 7;
const int trigPinIn = 8;
const int echoPinIn = 9;
const int trigPinOut = 10;
const int echoPinOut = 11;

//defines parameters
double maxDistance = 70;
double minDistance = 10;
int delayTimeMilliseconds = 400;
int maxCheckTime = 200;

// defines variables
int peopleIn = 0;
int peopleOut = 0;
int visitor = 0;
int pirCounter;

void setup() {
  pinMode(trigPinIn, OUTPUT); // Sets the trigPinIn as an Output
  pinMode(echoPinIn, INPUT); // Sets the echoPinIn as an Input
  pinMode(trigPinOut, OUTPUT); // Sets the trigPinOut as an Output
  pinMode(echoPinOut, INPUT); // Sets the echoPinOut as an Input
  pinMode(pirPin, INPUT);
  Serial.begin(115200); // Starts the serial communication
}

void pirSensor() {
  int val = digitalRead(pirPin);
  if (visitor == 0 && val == HIGH) {
    peopleIn++;
    pirCounter = 0;
  } else if (visitor > 0 && val == LOW && pirCounter >= 21000) {
    peopleOut = peopleIn;
    pirCounter = 0;
  } else if (visitor > 0 && val == HIGH) {
    pirCounter = 0;
  }
  pirCounter++;
}

double measureDistance(int trigPin, int echoPin) {
  double duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;
  return distance;
}

boolean checkDistance(double distance) {
  if (distance < maxDistance && distance > minDistance) {
    return true;
  }
  return false;
}

void loop() {
  boolean inCounter = checkDistance(measureDistance(trigPinIn, echoPinIn));
  boolean outCounter = checkDistance(measureDistance(trigPinOut, echoPinOut));
  boolean doubleRead = inCounter && outCounter;
  if (!doubleRead) {
    pirCounter = 0;
    if (inCounter) {
      delay(delayTimeMilliseconds);
      int delayTime = maxCheckTime;
      while (delayTime != 0) {
        outCounter = checkDistance(measureDistance(trigPinOut, echoPinOut));
        if (outCounter) {
          peopleIn++;
          delayTime = 1;
        }
        delayTime--;
      }
    } else if (outCounter) {
      delay(delayTimeMilliseconds);
      int delayTime2 = maxCheckTime;
      while (delayTime2 != 0) {
        inCounter = checkDistance(measureDistance(trigPinIn, echoPinIn));
        if (inCounter) {
          if (visitor == 0) {
            peopleOut = peopleOut;
          } else {
            peopleOut++;
          }
          delayTime2 = 1;
        }
        delayTime2--;
      }
    }
  }
  visitor = peopleIn - peopleOut;
  Serial.write(peopleIn);
  Serial.write("-");
  Serial.write(peopleOut);
  Serial.write("-");
  pirSensor();
}
