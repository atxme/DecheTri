#include <Servo.h>
#include <Stepper.h>

const int stepsPerRevolution = 200;
Stepper stepper(stepsPerRevolution, 7, 6, 5, 4);

Servo servo;
const int servoPin = 3;

const int trigPin = 9;
const int echoPin = 8;
long duration;
int distance;
//unsigned long time;
//unsigned long previousMicros = 0;
//unsigned long previousMillis = 0;
//int triggerState = LOW;
const int distanceDetection = 10;

int classification = 0;

int currentAngle = 0;
const int servoAngles[3] = {0, 45, 135};
const int stepperSteps[3] = {2000, 1000, 1000};

void setup() {
	Serial.begin(9600);
	Serial.setTimeout(1);
	stepper.setSpeed(60);
	pinMode(servoPin, OUTPUT);
	servo.attach(servoPin);
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
		delay(2000)
	}
}

int getDistance(){
	digitalWrite(trigPin, HIGH); // generate 10-microsecond pulse to TRIG pin
	delayMicroseconds(10);
	digitalWrite(trigPin, LOW);
	duration = pulseIn(echoPin, HIGH); // measure duration of pulse from ECHO pin
	distance = ((duration*0.034)/2);
	return(distance);
}

/*int getDistance(){
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
}*/

