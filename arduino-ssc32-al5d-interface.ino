// VAKIOT
const int numReadings = 3;
const int readDelay = 5;

// MUUTTUJAT
	// potikoiden lukemiseen
	int index1 = 0;
	int readings1[numReadings];
	int total1 = 0;
	int average1 = 0;
	
	int index2 = 0;
	int readings2[numReadings];
	int total2 = 0;
	int average2 = 0;
	

int switchBase = 0;
int baseValue = 1500;
int newBaseValue = 1500;

// alin servo
int switchShoulderForward = 0;
int switchShoulderBack = 0;
int shoulderValue = 1500; 

//kyynärpää
int switchElbowForward = 0;
int switchElbowBack = 0;
int elbowValue = 1500;

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
  
	for (int thisReading = 0; thisReading < numReadings; thisReading++) {
		readings1[thisReading] = 0;
	}
	
	for (int thisReading = 0; thisReading < numReadings; thisReading++) {
		readings2[thisReading] = 0;
	}
  
  delay(1000);
  startPosition();
  
// alustaa tarvittavat nappulat liikuttamiseen
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  pinMode(2, INPUT);
  
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
  
  switchShoulderBack = digitalRead(2);
  switchShoulderForward = digitalRead(3);
  switchElbowBack = digitalRead(4);
  switchElbowForward = digitalRead(5),
  switchWristUp = digitalRead(6);
  switchWristDown = digitalRead(7);
  switchBase = analogRead(A0);
  switchGrip = analogRead(A1);
     
  if(switchShoulderBack == HIGH){
    shoulderValue += transitionSpeed;
    updatePositions();
	delay(readDelay);
  }
  if(switchShoulderForward == HIGH){
    shoulderValue -= transitionSpeed;
    updatePositions();
	delay(readDelay);
  }
  if(switchElbowBack == HIGH){
    elbowValue -= transitionSpeed;
    updatePositions();
	delay(readDelay);
  }
  if(switchElbowForward == HIGH){
    elbowValue += transitionSpeed;
    updatePositions();
	delay(readDelay);
  }
  if(switchWristUp == HIGH){
    wristValue -= transitionSpeed;
    updatePositions();
	delay(readDelay);
  }
  
  if(switchWristDown == HIGH){
    wristValue += transitionSpeed;
    updatePositions();
  }
  
  newBaseValue = (runningAverageA0() * 1.956) + 500;
  if(baseValue != newBaseValue) {
	baseValue = newBaseValue;
	updatePositions();
  }
  
  
  
  newGripValue = (runningAverageA1() * 2.444);
  if(gripValue != newGripValue) {
	if (newGripValue > 1000) {
		gripValue = newGripValue;
		updatePositions();
	}
  }
  
}

void updatePositions(){
  String send = "#";
  Serial.print(send + 1 + "P" + baseValue );
  Serial.print(send + 2 + "P" + shoulderValue);
  Serial.print(send + 3 + "P" + elbowValue );
  Serial.print(send + 4 + "P" + wristValue );
  Serial.println(send + 5 + "P" + gripValue + "T" + 15);
  
  
}

int runningAverageA0() {
	// subtract the last reading:
   total1 = total1 - readings1[index1];         
   // read from the sensor:  
   readings1[index1] = analogRead(A0); 
   // add the reading to the total:
   total1 = total1 + readings1[index1];       
   // advance to the next position in the array:  
   index1 = index1 + 1;                    

   // if we're at the end of the array...
   if (index1 >= numReadings)              
     // ...wrap around to the beginning: 
     index1 = 0;                           

   // calculate the average:
   average1 = total1 / numReadings;
   delay(readDelay);
   return average1;
}

int runningAverageA1() {
	// subtract the last reading:
   total2 = total2 - readings2[index2];         
   // read from the sensor:  
   readings2[index2] = analogRead(A1); 
   // add the reading to the total:
   total2 = total2 + readings2[index2];       
   // advance to the next position in the array:  
   index2 = index2 + 1;                    

   // if we're at the end of the array...
   if (index2 >= numReadings)              
     // ...wrap around to the beginning: 
     index2 = 0;                           

   // calculate the average:
   average2 = total2 / numReadings;
   delay(readDelay);
   return average2;
}
