#include <math.h>

// VAKIOT
	const double baseHeight = 68;
	const double shoulderLength = 146;
	const double elbowLength = 186;
	const double handLength = 86;

// MUUTTUJAT
// kytkimet
	int switchBase = 0;
	int switchIncrementWristHeight = 0;
	int switchDecrementWristHeight = 0;
	int switchIncrementWristDistance = 0;
	int switchDecrementWristDistance = 0;
	int switchIncrementHandAngle = 0;
	int switchDecrementHandAngle = 0;
	int switchGrip = 0; 

// servojen parametrit
	int baseValue = 1500;
	int newBaseValue = 1500;
	int shoulderValue = 1550;
	int elbowValue = 1000;
	int wristValue = 2180;
	int gripValue = 1160;
	int newGripValue = 1160;

// käänteiskinematiikka
	// paikkakoordinaatit
	double CoordX;
	double CoordY;
	double CoordZ;
		
	// servokulmat
	double shoudlerAngleRad;
	double elbowAngleRad;
	double wristAngleRad;
	double baseAngleRad;
	
	// muut muuttujat
	double fingerRadius;
	double wristDistAxisZ;
	double wristShoulderLine;
	double angleHorizToWristShoulderLine;
	double angleWristShoulderLineToShoulderBone;
	
// Muutosparametrit
	int basicTime =2000;
	double transitionSpeed = 10.0;

void setup(){
  Serial.begin(9600);
  
  delay(1000);
  startPosition();
  
// alustaa tarvittavat nappulat liikuttamiseen
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  
// alustaa rullat, joilla ohjataan pohjan kiertoa ja kouraa
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
}

// ohjelman käynnistyessä robotti halutaan tiettyyn 
// alkupaikkaan
void startPosition(){
  String send = "#";
  Serial.print(send + 1 + "P" + baseValue );
  Serial.print(send + 2 + "P" + shoulderValue);
  Serial.print(send + 3 + "P" + elbowValue );
  Serial.print(send + 4 + "P" + wristValue );
  Serial.println(send + 5 + "P" + gripValue + "T" + basicTime);
}

void loop(){
  
	switchIncrementWristDistance = digitalRead(2);
	switchDecrementWristDistance = digitalRead(3);
	switchIncrementWristHeight = digitalRead(4);
	switchDecrementWristHeight = digitalRead(5);
	switchIncrementHandAngle = digitalRead(6);
	switchDecrementHandAngle = digitalRead(7);
	switchBase = analogRead(A0);
	switchGrip = analogRead(A1);
     
	if (switchIncrementWristDistance == HIGH) {
		CoordX += transitionSpeed;
		updatePositions();
	}
  if (switchDecrementWristDistance == HIGH) {
		CoordX -= transitionSpeed;
		updatePositions();
	}
  if (switchIncrementWristHeight == HIGH) {
		CoordY += transitionSpeed;
		updatePositions();
	}
  if (switchDecrementWristHeight == HIGH) {
		CoordY -= transitionSpeed;
		updatePositions();
	}
  if(switchIncrementHandAngle == HIGH){
		wristValue -= 0.10;
		updatePositions();
  }
  if(switchDecrementHandAngle == HIGH){
		wristValue += 0.10;
		updatePositions();
  }
  
  newBaseValue = (analogRead(A0) * 1.9) + 500;
  if(baseValue != newBaseValue) {
	baseValue = newBaseValue;
	updatePositions();
  }
  
  newGripValue = (analogRead(A1) * 2.4);
  if(gripValue != newGripValue) {
	if (newGripValue > 1000) {
		gripValue = newGripValue;
		updatePositions();
	}
  }
}

// METODIT

void updatePositions(){
	calculateServoValues();

	String send = "#";
	Serial.print(send + 1 + "P" + baseValue );
	Serial.print(send + 2 + "P" + shoulderValue);
	Serial.print(send + 3 + "P" + elbowValue );
	Serial.print(send + 4 + "P" + wristValue );
	Serial.println(send + 5 + "P" + gripValue + "T" + 15);
}

void calculateServoValues() {
	
}

void updateFingerRadius() {
	radius = pow(CoordX, 2) + pow(CoordY);
}

void updateWristDistAxisZ() {
	wristDistanceFromAxisZ = fingerRadius - handLength;
}

void updateWristShoulderLine() {
	wristShoulderLine = sqrt(pow(wristDistanceFromAxisZ, 2) + pow(CoordZ, 2));
}

void updateAngleHorizToWristShoulderLine() {
	atan2(CoordZ, wristDistAxisZ);
}

void updateAngleWristShoulderLineToShoulderBone() {
	angleWristShoulderLineToShoulderBone = acos((pow(shoulderLength, 2) + pow(wristShoulderLine, 2) + pow(elbowLength, 2)) / (2 * shoulderLength * wristShoulderLine));
}

void updateShoudlerAngleRad() {
	shoudlerAngleRad = angleHorizToWristShoulderLine + angleWristShoulderLineToShoulderBone;
}

void updateElbowAngleRad() {
	elbowAngleRad =  acos(((elbowLength^2)+(shoulderLength^2)-(wristShoulderLine^2)) / (2*shoulderLength*elbowLength));
}

void updateWristAngleRad() {
	wristAngleRad = -elbowAngleRad - shoudlerAngleRad + (2 * PI);
}

void updateBaseAngleRad() {
	baseAngleRad = atan2(CoordX, CoordY);
}

//

// void printInfo() {
	// String emptyStr = "";
	// Serial.println(emptyStr + "Shoulder: " + shoulderValue + " Elbow: " + elbowValue + " Theta1: " + Theta1 + " Theta2: " + Theta2);
// }