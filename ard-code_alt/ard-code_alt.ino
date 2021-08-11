/*
 * 
 * Final code
 * 
 * Connect ard-rpi
 * 
 * ard
 * ---
 * 
 * pin2 left
 * pin3 right
 * 
 * rpi
 * ---
 * gpio14 left
 * gpio15 right
 * 
 * sensor 1
 * --------
 * 
 * trig 7
 * eco 8
 * 
 * sensor 2
 * --------
 * trig2 11 
 * eco2 12
 * 
 * 
 * with two modes
 * ---------------
 * Mode A => "follow host"
 * pin 4
 * Mode B => "create 3d"
 * pin 13
 * Made by Kenny
 */

const int interpin1 = 2;
const int interpin2 = 3;

const int motI = 10;
const int motD = 5;
const int dirI = 9;
const int dirD = 6;

int trig = 7;
int eco = 8;
int trig2 = 11;
int eco2 = 12;

int modA = 4;
int modB = 13;

long duration;
int distance;

//velocidad
int x = 70;
int y = 60;

void setup() {
  Serial.begin(9600);

  pinMode(interpin1, INPUT_PULLUP);
  pinMode(interpin2, INPUT_PULLUP);
  pinMode(modA, INPUT_PULLUP);
  pinMode(modB, INPUT_PULLUP);
  
  pinMode(trig,OUTPUT);
  pinMode(eco, INPUT);
  pinMode(trig2,OUTPUT);
  pinMode(eco2,INPUT);

  pinMode(motI, OUTPUT);
  pinMode(motD, OUTPUT);
  pinMode(dirI, OUTPUT);
  pinMode(dirD, OUTPUT);

  digitalWrite(motI, LOW);
  digitalWrite(motD, LOW);
  digitalWrite(dirI, HIGH);
  digitalWrite(dirD, LOW);
}

void loop() {
  //testing code
  /*
  if(Serial.available() >0){
    char a = Serial.read();
    switch (a){
      case 'a':
        der(450);
        dist (trig,eco);
        Serial.println(distance);
      break;
      case 'b':
        left(450);
        dist(trig2,eco2);
        Serial.println(distance);
       break;
    }
  }*/
  //Form code 
  
  while(digitalRead(modA) == 0){
    //tracking
    Serial.println("tracking");
    modeA();
  }
  while(digitalRead(modB) == 0){
    //3d Model
    Serial.println("3d model");
    modeB();
  }
}
void modeB(){
  dist(trig,eco);
  if(distance < 90){
    Serial.println("b - giro");
    der(500);
    delay(250);
  }else if(distance > 90){
    Serial.println("b- adelante");
    adelante(380);
  }
}
//Sub rutina for follow
void modeA(){  
  dist(trig, eco);
  if(distance > 2500 || distance < 5){
    while(digitalRead(2) == LOW){
      Serial.println("left");
      left(300);
    }
    delay(200);
    while(digitalRead(3) == LOW){
      Serial.println("right");
      der(300);
    }
    delay(200);
  }else{
    if(distance > 120){
      if(distance > 150){
        if(distance > 155){
          adelante(500);
          delay(200);
          dist(trig, eco); 
        }else{
          while(digitalRead(2) == LOW){
            Serial.println("left");
            left(300);
          }
          delay(200);
          while(digitalRead(3) == LOW){
            Serial.println("right");
            der(300);
          }
          delay(200);
        }
      }else{
        para();
        delay(200);
      }
    }else{
      if(distance < 120 && distance > 6){
        dist(trig2,eco2);
        delay(200);
        if(distance > 40){
          atras();
          delay(100);
          para();
          distance = 0;
          dist(trig, eco);
          delay(200);
        }else{
          dist(trig, eco);
        }
      }
    }
  }  
}
void adelante(int a){
  //Serial.println("adelante");
  digitalWrite(dirD, HIGH);
  digitalWrite(dirI, LOW);
  delay(50);
  analogWrite(motI, y);
  analogWrite(motD, y);
  delay(a);
  analogWrite(motI, 0);
  analogWrite(motD, 0);
  delay(50);
}

void atras() {
  digitalWrite(dirD, LOW);
  digitalWrite(dirI, HIGH);
  delay(50);
  analogWrite(motI, y);
  analogWrite(motD, y);
  delay(400);
  analogWrite(motI, 0);
  analogWrite(motD, 0);
  delay(50);
}

void left(int a) {
  digitalWrite(dirD, HIGH);
  digitalWrite(dirI, HIGH);
  delay(50);
  analogWrite(motI, x);
  analogWrite(motD, x);
  delay(a);
  analogWrite(motI, 0);
  analogWrite(motD, 0);
  delay(50);
}
void der(int a){
  digitalWrite(dirD, LOW);
  digitalWrite(dirI, LOW);
  delay(50);
  analogWrite(motI, x);
  analogWrite(motD, x);
  delay(a);
  analogWrite(motI, 0);
  analogWrite(motD, 0);
  delay(50);
}

void para() {
  digitalWrite(motI,0);
  digitalWrite(motD,0);
}

void dist (int a, int b) {
  digitalWrite(a, HIGH);
  delayMicroseconds(2);
  digitalWrite(a, LOW);
  delayMicroseconds(5);
  digitalWrite(a, HIGH);
  duration = pulseIn(b, HIGH);
  distance = duration * 0.017;
}
