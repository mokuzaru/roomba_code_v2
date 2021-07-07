/*
 * Connect 
 * 
 * ard- rpi
 * 
 * ard
 * ---
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
 * trig 7
 * eco 8
 * 
 * Sensor 2
 * -------
 * trig2 11
 * eco2 12
 */

const int interpin1 = 2;
const int interpin2 = 3;

const int motI = 10; //pwm1
const int motD = 5;  //pwm2
const int dirI = 9;
const int dirD = 6;

int trig = 7;
int eco = 8;
int trig2 = 11;
int eco2 = 12;

long duration;
int distance;

//Velocidad 
int x = 90;
int y = 80;

void setup() {
  Serial.begin(9600);
  pinMode(13,OUTPUT);
  pinMode(interpin1, INPUT_PULLUP);
  pinMode(interpin2, INPUT_PULLUP);

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
/*
  left();
  delay(1500);
  der();
  delay(1500);
  */
}

void loop() {

  while(digitalRead(2) == 0){
    left();
  }
  while(digitalRead(3) == 0){
    der();
  }
  dist(trig, eco);
  
  if(distance < 50 || distance > 150){
    para();
  }else if(digitalRead(2) == 1 || digitalRead(3)== 1){
    if(distance >120 && distance < 150){
      adelante();
    }
    if(distance < 80 && distance > 50){
      dist(trig2,eco2);
      Serial.print("distancia atras: ");
      Serial.println(distance);
      if(distance > 40){
        atras();
        distance = 0;   
      }      
    }
  }
  delay(500);
}

void adelante(){
  Serial.println("adelante");
  digitalWrite(dirD, LOW);
  digitalWrite(dirI, HIGH);
  delay(50);
  analogWrite(motI, x);
  analogWrite(motD, x);
  delay(700);
  analogWrite(motI, 0);
  analogWrite(motD, 0);
  delay(50);
}

void atras() {
  Serial.println("atras");
  digitalWrite(dirD, HIGH);
  digitalWrite(dirI, LOW);
  delay(50);
  analogWrite(motI, x);
  analogWrite(motD, x);
  delay(700);
  analogWrite(motI, 0);
  analogWrite(motD, 0);
  delay(50);
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
