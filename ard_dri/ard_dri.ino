/*
 * Connect 
 * 
 * ard- rpi
 * 
 * ard
 * pin2 left
 * pin3 right 
 * 
 * rpi
 * gpio14 left
 * gpio15 right
 * 
 */

const int interpin1 = 2;
const int interpin2 = 3;

const int motI = 10; //pwm1
const int motD = 5;  //pwm2
const int dirI = 9;
const int dirD = 6;

int trig = 7;
int eco = 8;

long duration;
int distance;

//Velocidad 
int x = 90;
int y = 80;

void setup() {
  Serial.begin(9600);
  pinMode(13,OUTPUT);
  pinMode(interrpin1, INPUT_PULLUP);
  pinMode(interrpin2, INPUT_PULLUP);

  pinMode(trig,OUTPUT);
  pinMode(eco, INPUT);

  pinMode(motI, OUTPUT);
  pinMode(motD, OUTPUT);
  pinMode(dirI, OUTPUT);
  pinMode(dirD, OUTPUT);

  digitalWrite(motI, LOW);
  digitalWrite(motD, LOW);
  digitalWrite(dirI, HIGH);
  digitalWrite(dirD, LOW);

  left();
  delay(1500);
  der();
  delay(1500);
}

void loop() {
  // put your main code here, to run repeatedly:
  while(digitalRead(2) == 0){
    left();
  }
  while(digitalRead(3) == 0){
    der();
  }
}

void left() {
  Serial.println("left");
  digitalWrite(dirD, LOW);
  digitalWrite(dirI, LOW);
  delay(50);
  analogWrite(motI, y);
  analogWrite(motD, y);
  delay(700);
  analogWrite(motI, 0);
  analogWrite(motD, 0);
  delay(50);
  
  digitalWrite(13,HIGH);
  delay(200);
  digitalWrite(13,LOW);
}
void der(){
  Serial.println("right");
  digitalWrite(dirD, HIGH);
  digitalWrite(dirI, HIGH);
  delay(50);
  analogWrite(motI, y);
  analogWrite(motD, y);
  delay(700);
  analogWrite(motI, 0);
  analogWrite(motD, 0);
  delay(50);
  
  digitalWrite(13,HIGH);
  delay(200);
  digitalWrite(13,LOW);
}

void para() {
  digitalWrite(motI,0);
  digitalWrite(motD,0);
  digitalWrite(13,LOW);
  Serial.println("paro");
}

void dist (int a, int b) {
  Serial.println("start medida");

  digitalWrite(a, HIGH);
  delayMicroseconds(2);
  digitalWrite(a, LOW);
  delayMicroseconds(5);
  digitalWrite(a, HIGH);

  duration = pulseIn(b, HIGH);

  distance = duration * 0.017;

  Serial.print("Distancia: ");
  Serial.println(distance);
}
