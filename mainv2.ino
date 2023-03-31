#include <Servo.h>
#include <Stepper.h>

const int stepsPerRevolution = 200;
Stepper stepper(stepsPerRevolution, 7, 6, 5, 4);

Servo servo;

const int trigPin = 9;
const int echoPin = 8;
long duration;
int distance;
unsigned long time;
unsigned long previousMicros = 0;
unsigned long previousMillis = 0;
int triggerState = LOW;
const int distanceDetection = 10;

int classification = 2;
bool waitclassification = true;

int currentAngle = 50;
const int servoAngles[3] = {0, 45, 135};
const int stepperSteps[3] = {2000, 1000, 1000};

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(1);
  stepper.setSpeed(60);
  pinMode(3, OUTPUT);
  servo.attach(3);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  servo.write(currentAngle);
}

void loop() {
  while (!Serial.available());
  if(getDistance() < distanceDetection){
    classification = Serial.readString().toInt();
    if(servoAngles[classification] != currentAngle){
      servo.write(servoAngles[classification]);
    }
    stepper.step(stepperSteps[classification]);
  }
}

int getDistance(){
  unsigned long currentMicros = micros();
  if((triggerState == LOW) && (currentMicros - previousMicros >= 2)){
    triggerState = HIGH; // turn it on
     previousMicros = currentMicros; // remember the time
     digitalWrite(trigPin, triggerState); // update the actual trigger
  } else if((triggerState == HIGH) && (currentMicros - previousMicros >= 10)){           
    triggerState = LOW; // turn it off
    previousMicros = currentMicros; // remember the time
    digitalWrite(trigPin, triggerState); // update the actual trigger
  }
  duration = pulseIn(echoPin, HIGH);
  if(duration > 0.001){
    distance = ((duration*0.034)/2);
    return(distance);
  }
}

