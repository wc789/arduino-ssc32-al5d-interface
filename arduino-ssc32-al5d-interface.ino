// switch.. kertoo onko nappi painettu

#include <math.h>

//---------------------------------VAKIOT-------------------------------------
	const double baseHeight = 68;
	const double shoulderLength = 146;
	const double elbowLength = 186;
	const double handLength = 86;

//--------------------------------MUUTTUJAT-----------------------------------
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

// servojen rajat
	// double shoulderLimitAngleLower = 0;
	// double elbowLimitAngleLower = 20 * PI / 180;

// käänteiskinematiikka
	// paikkaparametrit
	double CoordX = 100;
	double CoordY = 100;
	
	double wristDistanceX = 100;
	double wristDistanceY = 100;
	double handAngle = 0;
	double wristDistance = sqrt(1 / (pow(wristDistanceX, 2) + pow((wristDistanceY),2)) - 1);
	
	// kulmat
	double theta1 = 1;
	double theta2 = 1;
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
	theta2 = calcTheta2();
	theta1 = calcTheta1();
	
	String emptyStr = "";
	Serial.println(emptyStr + "Shoulder: " + shoulderValue + " Elbow: " + elbowValue + " Theta1: " 
	+ theta1 + " Theta2: " + theta2 + " WristHeight: " + wristDistanceY + " WristDistance: " + wristDistanceX);
	
	elbowValue = 1500 / PI * theta2 + 1000;
	shoulderValue = 1700 / PI * theta1 + 700;
}

double calcWristDistanceX() {
	return ((CoordX - handLength * cos(handAngle)) / (2 * shoulderLength));
}

double calcWristDistanceY() {
	return ((CoordY - baseHeight - handLength * cos(handAngle)) / (2 * shoulderLength));
}

double calcFingerDistance() {
	return sqrt(1 / (pow(calcWristDistanceX(), 2) + pow(calcWristDistanceY(), 2)) - 1);
}

double calcShoulderGroundAngle() {
	return atan2(calcWristDistanceX() - calcFingerDistance() * calcWristDistanceY(), calcWristDistanceY() + calcFingerDistance() * calcWristDistanceX());
}

double calcElbowGroundAngle() {
	return atan2(calcWristDistanceX() + calcFingerDistance() * calcWristDistanceY(), calcWristDistanceY() - calcFingerDistance() * calcWristDistanceX());
}

double calcTheta1() {
	double subResult = calcShoulderGroundAngle() - PI;
	double result = subResult + subResult;
	return result;
}

double calcTheta2() {
	double subResult = calcElbowGroundAngle() - calcTheta1();
	double result = subResult + subResult;
	return result;
}

double calcTheta3() {
	double subResult = handAngle - calcElbowGroundAngle();
	double result = subResult + subResult;
	return result;
}

// void printInfo() {
	// String emptyStr = "";
	// Serial.println(emptyStr + "Shoulder: " + shoulderValue + " Elbow: " + elbowValue + " Theta1: " + Theta1 + " Theta2: " + Theta2);
// }