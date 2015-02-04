// switch.. kertoo onko nappi painettu

//pohja
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
  }
  if(switchShoulderForward == HIGH){
    shoulderValue -= transitionSpeed;
    updatePositions();
  }
  if(switchElbowBack == HIGH){
    elbowValue -= transitionSpeed;
    updatePositions();
  }
  if(switchElbowForward == HIGH){
    elbowValue += transitionSpeed;
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
  
  newBaseValue = (analogRead(A0) * 2) + 500;
  if(baseValue != newBaseValue) {
	baseValue = newBaseValue;
	updatePositions();
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