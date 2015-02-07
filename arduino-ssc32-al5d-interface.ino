// switch.. kertoo onko nappi painettu

#include <math.h>

//pohja
int switchBase = 0;
int newBaseValue = 1500;
int baseValue = 1500;
double baseHeight = 6.8;

// Olkapää
int shoulderValue = 0;
double shoulderLength = 14.6;
double shoulderLimitAngleLower = 0;

// Kyynärpää
int elbowValue = 1500;
double elbowLength = 18.6;
double elbowLimitAngleLower = 20 * PI / 180;

// Ranteen korkeus
int switchIncrementWristHeight = 0;
int switchDecrementWristHeight = 0;
double WristHeight = 20;
double WristHeightPrime = WristHeight - baseHeight;

// Ranteen etäisyys
int switchIncrementWristDistance = 0;
int switchDecrementWristDistance = 0;
double WristDistance = 20;		// Jotain näihin arvoiksi, arvot mm?

// Ranteen asento
int switchWristDown = 0;
int switchWristUp = 0;
int wristValue = 1500;

// Koura
int switchGrip = 0; 
int gripValue = 1100;
int newGripValue = 1100;


//Vakio aika 1 sekunti
int basicTime =2000;
int transitionSpeed = 0.1;

void setup(){
  Serial.begin(9600);
  
  delay(1000);
  updatePositions();
  
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
	switchWristUp = digitalRead(6);
	switchWristDown = digitalRead(7);
	switchBase = analogRead(A0);
	switchGrip = analogRead(A1);
     
	if (switchIncrementWristDistance == HIGH) {
		WristDistance += transitionSpeed;
		updatePositions();
	}
  if (switchDecrementWristDistance == HIGH) {
		WristDistance -= transitionSpeed;
		updatePositions();
	}
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
  
  newBaseValue = (analogRead(A0) * 1.955) + 500;
  if(baseValue != newBaseValue) {
	baseValue = newBaseValue;
	updatePositions();
  }
  
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
	
	// Ehtolauseet Thetojen rajoille
	if ((Theta1 >= shoulderLimitAngleLower) && (Theta2 >= elbowLimitAngleLower)) {
		elbowValue = 1500 / PI * Theta2 + 1000;
		shoulderValue = 1700 / PI * Theta1 + 700;
	} else if ((Theta2 < elbowLimitAngleLower) && (Theta1 < shoulderLimitAngleLower)) {
		elbowValue = 1000;
		shoulderValue = 700;
	} else if ((Theta1 >= shoulderLimitAngleLower) && (Theta2 < elbowLimitAngleLower)) {
		elbowValue = 1000;
		shoulderValue = 1700 / PI * Theta1 + 700;
	} else if ((Theta1 < shoulderLimitAngleLower) && (Theta2 >= elbowLimitAngleLower)) {
		elbowValue = 1500 / PI * Theta2 + 1000;
		shoulderValue = 700;
	}
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