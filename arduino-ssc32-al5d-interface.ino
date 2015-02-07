// switch.. kertoo onko nappi painettu

#include <math.h>

//pohja
//int switchBase = 0;
//int newBaseValue = 1500;
int baseValue = 1500;
int baseHeight = 10;			//TODO: Oikeat arvot tähän

// alin servo
// int switchShoulderForward = 0;
// int switchShoulderBack = 0;
int shoulderValue = 1500;
int shoulderLength = 20;		//TODO: Oikeat arvot tähän

//kyynärpää
// int switchElbowForward = 0;
// int switchElbowBack = 0;
int elbowValue = 1500;
int elbowLength = 20;			//TODO: Oikeat arvot tähän

int switchIncrementX = 0;
int switchDecrementX = 0;
int CoordinateX = 0;		// Jotain näihin arvoiksi, arvot mm?

int switchIncrementWristHeight = 0;
int switchDecrementWristHeight = 0;
int WristHeight = 0;		// Jotain näihin arvoiksi, arvot mm?
int WristHeightPrime = WristHeight - baseHeight;

int switchIncrementWristDistance = 0;
int switchDecrementWristDistance = 0;
int WristDistance = 0;		// Jotain näihin arvoiksi, arvot mm?

//ranne 
int switchWristDown = 0;
int switchWristUp = 0;
int wristValue = 1500;

//koura 
int switchGrip = 0; 
int gripValue = 1100;
int newGripValue = 1100;


//Vakio aika 1 sekunti
int basicTime =2000;
int transitionSpeed = 10;

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
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  
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
  // switchShoulderBack = digitalRead(2);
  switchDecrementWristDistance = digitalRead(3);
  // switchShoulderForward = digitalRead(3);
  switchIncrementX = digitalRead(4);
  // switchElbowBack = digitalRead(4);
  switchDecrementX = digitalRead(5);
  // switchElbowForward = digitalRead(5),
  switchIncrementWristHeight = digitalRead(6);
  switchDecrementWristHeight = digitalRead(7);
  switchWristUp = digitalRead(8);
  switchWristDown = digitalRead(9);
  //switchBase = analogRead(A0);
  switchGrip = analogRead(A1);
     
	if (switchIncrementWristDistance == HIGH) {
		WristDistance += transitionSpeed;
		updatePositions();
	}
  // if(switchShoulderBack == HIGH){
    // shoulderValue += transitionSpeed;
    // updatePositions();
  // }
  if (switchDecrementWristDistance == HIGH) {
		WristDistance -= transitionSpeed;
		updatePositions();
	}
  // if(switchShoulderForward == HIGH){
    // shoulderValue -= transitionSpeed;
    // updatePositions();
  // }
  if (switchIncrementX == HIGH) {
		CoordinateX += transitionSpeed;
		updatePositions();
	}
  // if(switchElbowBack == HIGH){
    // elbowValue -= transitionSpeed;
    // updatePositions();
  // }
  if (switchDecrementX == HIGH) {
		CoordinateX -= transitionSpeed;
		updatePositions();
	}
  // if(switchElbowForward == HIGH){
    // elbowValue += transitionSpeed;
    // updatePositions();
  // }
  if (switchIncrementWristHeight == HIGH) {
		WristHeight += transitionSpeed;
		updatePositions();
	}
  if (switchDecrementWristHeight == HIGH) {
		WristHeight -= transitionSpeed;
		updatePositions();
	}
  if(switchWristUp == HIGH){
		wristValue -= transitionSpeed;
		updatePositions();
  }
  
  if(switchWristDown == HIGH){
		wristValue += transitionSpeed;
		updatePositions();
  }
  
  // newBaseValue = (analogRead(A0) * 1.955) + 500;
  // if(baseValue != newBaseValue) {
	// baseValue = newBaseValue;
	// updatePositions();
  // }
  
  newGripValue = (analogRead(A1) * 2.444);
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
	WristHeightPrime = WristHeight - baseHeight;
	double Theta2 = calculateTheta2(WristDistance, WristHeightPrime);
	double Theta1 = calculateTheta1(WristDistance, WristHeightPrime);
	double Theta0 = calculateTheta0(CoordinateX, WristDistance);
	
	// TODO: Tarkistukset, jotta vain sallituissa rajoissa olevat kulmat muutetaan!
	// TODO: Tähän kaavat Thetojen muuttamiseksi servojen arvoiksi!
}

double calculateTheta2(int X, int Y) {
	double baseArg((X * X + Y * Y - shoulderLength * shoulderLength - elbowLength * elbowLength)
					/ 2 * shoulderLength * elbowLength);
	return atan2(sqrt(1 - baseArg), baseArg);
}

double calculateTheta1(int X, int Y) {
	double k1 = shoulderLength + elbowLength * cos(calculateTheta2(X, Y));
	double k2 = elbowLength * sin(calculateTheta2(X, Y));
	return atan2(Y, X) - atan2(k2, k1);
}

double calculateTheta0(int X, int Y) {
	return atan2(Y, X);
}