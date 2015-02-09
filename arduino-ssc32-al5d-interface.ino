#include <math.h>

// VAKIOT
	const double baseHeight = 6.8;
	const double shoulderLength = 14.6;
	const double elbowLength = 18.6;
	const double handLength = 8.6;

// MUUTTUJAT
// kytkimet
	int switchBase = 0;
	int switchIncrementCoordX = 0;
	int switchDecrementCoordX = 0;
	int switchIncrementWristAngle = 0;
	int switchDecrementWristAngle = 0;
	int switchIncrementCoordZ = 0;
	int switchDecrementCoordZ = 0;
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
	double CoordX = 0.2;
	double CoordY = 0.2;
	double CoordZ = 0.2;
		
	// servokulmat
	double shoulderAngleRad = 0;
	double elbowAngleRad = 0;
	// double wristAngleRad = 0;
	// double baseAngleRad = 0;
	
	// muut muuttujat
	double fingerRadius = 0;
	double wristDistFromAxisZ = 0;
	double wristShoulderLine = 0;
	double angleHorizToWristShoulderLine = 0;
	double angleWristShoulderLineToShoulderBone = 0;
	
// Muutosparametrit
	int basicTime =2000;
	double transitionSpeed = 0.01;

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
  
	switchIncrementCoordX = digitalRead(2);
	switchDecrementCoordX = digitalRead(3);
	switchIncrementCoordZ = digitalRead(4);
	switchDecrementCoordZ = digitalRead(5);
	switchIncrementWristAngle = digitalRead(6);
	switchDecrementWristAngle = digitalRead(7);
	switchBase = analogRead(A0);
	switchGrip = analogRead(A1);
     
	if (switchIncrementCoordX == HIGH) {
		CoordX += transitionSpeed;
		updatePositions();
	}
  if (switchDecrementCoordX == HIGH) {
		CoordX -= transitionSpeed;
		updatePositions();
	}
  if (switchIncrementWristAngle == HIGH) {
		wristValue -= 10;
		updatePositions();
	}
  if (switchDecrementWristAngle == HIGH) {
		wristValue += 10;
		updatePositions();
	}
  if(switchIncrementCoordZ == HIGH){
		CoordZ += transitionSpeed;
		updatePositions();
  }
  if(switchDecrementCoordZ == HIGH){
		CoordZ -= transitionSpeed;
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
	updateAngles();
	calculateServoValues();

	printInfo();
	
	if (baseValue < 500) {
		baseValue = 500;
	} else if (baseValue > 2500) {
		baseValue = 2500;
	}
	
	if (shoulderValue < 500) {
		shoulderValue = 500;
	} else if (shoulderValue > 2500) {
		shoulderValue = 2500;
	}
	
	if (elbowValue < 500) {
		elbowValue = 500;
	} else if (elbowValue > 2500) {
		elbowValue = 2500;
	}
	
	if (wristValue < 500) {
		wristValue = 500;
	} else if (wristValue > 2500) {
		wristValue = 2500;
	}
	
	String send = "#";
	Serial.print(send + 1 + "P" + baseValue );
	Serial.print(send + 2 + "P" + shoulderValue);
	Serial.print(send + 3 + "P" + elbowValue );
	Serial.print(send + 4 + "P" + wristValue );
	Serial.println(send + 5 + "P" + gripValue + "T" + 15);
}

void calculateServoValues() {
	// baseValue = degrees(baseAngleRad) * 9.833;
	shoulderValue = degrees(shoulderAngleRad) * 9.222;
	elbowValue = degrees(elbowAngleRad) * -9.444;
	// wristValue = degrees(wristAngleRad) * 10;
}

void updateAngles() {
	// updateFingerRadius();
	updateWristDistAxisZ();
	updateWristShoulderLine();
	updateAngleHorizToWristShoulderLine();
	updateAngleWristShoulderLineToShoulderBone();
	
	updateShoulderAngleRad();
	updateElbowAngleRad();
	// updateWristAngleRad();
	// updateBaseAngleRad();
}

// void updateFingerRadius() {
	// fingerRadius = pow(CoordX, 2) + pow(CoordY, 2);
// }	

void updateWristDistAxisZ() {
	wristDistFromAxisZ = CoordX;
}

void updateWristShoulderLine() {
	wristShoulderLine = sqrt(pow(wristDistFromAxisZ, 2) + pow(CoordZ, 2));
}

void updateAngleHorizToWristShoulderLine() {
	angleHorizToWristShoulderLine = atan2(CoordZ, wristDistFromAxisZ);
}

void updateAngleWristShoulderLineToShoulderBone() {
	angleWristShoulderLineToShoulderBone = acos(((pow(shoulderLength, 2)) + (pow(wristShoulderLine, 2)) - (pow(elbowLength, 2))) / (2 * shoulderLength * wristShoulderLine));
}

void updateShoulderAngleRad() {
	shoulderAngleRad = angleHorizToWristShoulderLine + angleWristShoulderLineToShoulderBone;
}

void updateElbowAngleRad() {
	elbowAngleRad =  acos(((pow(elbowLength, 2)) + (pow(shoulderLength, 2)) - (pow(wristShoulderLine, 2))) / (2 * shoulderLength * elbowLength));
}

// void updateWristAngleRad() {
	// wristAngleRad = -(elbowAngleRad) - shoulderAngleRad + (2 * PI);
// }

// void updateBaseAngleRad() {
	// baseAngleRad = atan2(CoordX, CoordY);
// }

void printInfo() {
	String emptyStr = " ";
	Serial.print(emptyStr + fingerRadius + " " + wristDistFromAxisZ + " " + wristShoulderLine + " " + angleHorizToWristShoulderLine + " " + angleWristShoulderLineToShoulderBone);
	Serial.print(emptyStr + " " + shoulderAngleRad + " " + elbowAngleRad + " " + " ");
	Serial.print(emptyStr + CoordX + emptyStr + CoordY + emptyStr + CoordZ);
}
