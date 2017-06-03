/*
* Ultrasonic Sensor HC-SR04 and Arduino Tutorial
*
* Crated by Dejan Nedelkovski,
* www.HowToMechatronics.com
*
*/
// defines pins numbers
int inputPin = 7; 
const int trigPinIn = 8;
const int echoPinIn = 9;
const int trigPinOut = 10;
const int echoPinOut = 11;

// defines variables

long durationIn;
double distanceIn;
long durationOut;
double distanceOut;

int visitor;
int visitorPrev;

boolean inCounter;
boolean outCounter;
boolean doubleRead = false;
int state;
int val = 0;
int pirCounter;

void setup() {
  pinMode(trigPinIn, OUTPUT); // Sets the trigPinIn as an Output
  pinMode(echoPinIn, INPUT); // Sets the echoPinIn as an Input
  pinMode(trigPinOut, OUTPUT); // Sets the trigPinOut as an Output
  pinMode(echoPinOut, INPUT); // Sets the echoPinOut as an Input
  pinMode(inputPin, INPUT);
  visitor = 0;
  visitorPrev = 0;
  doubleRead = false;
  state = 0;
  Serial.begin(9600); // Starts the serial communication
}

void pirSensor(){
  val = digitalRead(inputPin);
  if(visitor == 0 && val == HIGH && pirCounter > 500){
    visitor++;
    pirCounter = 0;
  }
  if(visitor > 0 && val == LOW && pirCounter >= 21000){
    visitor = 0;
    pirCounter = 0;
  }
  if(visitor > 0 && val == HIGH){
    pirCounter = 0;
  }
}

void inSensor(){
  // Clears the trigPinIn
  digitalWrite(trigPinIn, LOW);
  delayMicroseconds(2);
  // Sets the trigPinIn on HIGH state for 10 micro seconds
  digitalWrite(trigPinIn, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinIn, LOW);
  // Reads the echoPinIn, returns the sound wave travel time in microseconds
  durationIn = pulseIn(echoPinIn, HIGH);
  // Calculating the distance
  distanceIn = durationIn * 0.034 / 2;
  if(distanceIn > 10 && distanceIn < 150){
    inCounter = true;
  }
  else {
    inCounter = false;
  }
}

void outSensor(){
  // Clears the trigPinIn
  digitalWrite(trigPinOut, LOW);
  delayMicroseconds(2);
  // Sets the trigPinIn on HIGH state for 10 micro seconds
  digitalWrite(trigPinOut, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinOut, LOW);
  // Reads the echoPinIn, returns the sound wave travel time in microseconds
  durationOut = pulseIn(echoPinOut, HIGH);
  // Calculating the distance
  distanceOut = durationOut * 0.034 / 2;

  if(distanceOut > 10 && distanceOut < 150){
    outCounter = true;
  }
  else {
    outCounter = false;
  }
}


void loop() {
  inSensor();
  outSensor();
  /*Serial.println(distanceIn);
  Serial.println("Sensor that which located outside: ");
  Serial.println(distanceOut);*/
  if(!doubleRead){
    if(inCounter){
      pirCounter = 0;
      delay(300);
      int delayTime = 200;
      while(delayTime != 0){
        //inSensor();
        outSensor();
        if(outCounter){
          delay(200);
          visitor++;
          outCounter = false;
          delayTime = 1;
        }
        delayTime--;
      }
    }
    else if(outCounter){
      pirCounter = 0;
      delay(200);
      int delayTime2 = 200;
      while(delayTime2 != 0){
        inSensor();
        //outSensor();
        if(inCounter){
          delay(200);
          if(visitor == 0){
            visitor = visitor;
          }
          else{
            visitor--;
          }
          inCounter = false;
          delayTime2 = 1;
        }
        delayTime2--;
      }
    }
  }

  inSensor();
  outSensor();
  if(outCounter && inCounter){
    doubleRead = true;
  }
  else{
    doubleRead = false;
  }

  if(visitor != visitorPrev){
    Serial.print("Visitor: ");
    Serial.println(visitor);
    visitorPrev = visitor;
  }
  delayMicroseconds(40);
  //delay(500);
  pirSensor();
  pirCounter++;
}
