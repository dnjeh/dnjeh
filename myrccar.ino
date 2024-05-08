#include <SoftwareSerial.h>
#include <AFMotor.h>

AF_DCMotor motor_L(1);
AF_DCMotor motor_R(4);

#define TrigPin A0
#define EchoPin A1

#define Lspeed 145
#define Rspeed 200
long duration, distance, f=0, cnt=2;
SoftwareSerial BTSerial = SoftwareSerial(A5, A4);

void setup() {
  pinMode(EchoPin, INPUT);
  pinMode(TrigPin, OUTPUT);

  motor_L.setSpeed(0);
  motor_L.run(RELEASE);
  motor_R.setSpeed(0);
  motor_R.run(RELEASE);

  Serial.begin(9600);
  BTSerial.begin(9600);
}

void loop() {
  if(BTSerial.available()) {
    char val = BTSerial.read();
    if(val=='f') {
      go_motor();
      f=0;
    }
    else if(val=='b') {
      back_motor();
      f=0;
    }
    else if(val=='r') {
      right_motor();
      f=0;
    }
    else if(val=='l') {
      left_motor();
      f=0;
    }
    else if(val=='s') {
      stop_motor();
      f=0;
    }
    else if(val=='o') {
      f=1;
    }
    else if(val=='d') {
      gak_motor(++cnt);
      f=0;
    }
    else if(val=='c') {
      cnt=2;
      f=0;
    }
    //else stop_motor();
  }
  if(f) {
    go_motor();
    delay(50);
    Obstacle_Check();
  }
}

void gak_motor(int t) {
  for(int i=0;i<t;i++) {
    go_motor();
    delay(1000);
    ghlwjs(180-(180*(t-2)/t));
  }
}

void ghlwjs(int t) {
  motor_L.setSpeed(Lspeed);
  motor_L.run(FORWARD);
  motor_R.run(RELEASE);
  delay(t*7);
  stop_motor();
}

void Obstacle_Check() {
  int val = random(2);
  Distance_Measurement();
  delay(50);
  while(distance < 300) {
    if(distance < 150) {
      back_motor();
      delay(800);
      stop_motor();
      delay(50);
      Distance_Measurement();
      delay(100);
    }
    else {
      if(!val) {
        right_motor();
        delay(400);
      }
      else {
        left_motor();
        delay(400);
      }
      Distance_Measurement();
      delay(100);
    }
  }
}
void Distance_Measurement() {
  digitalWrite(TrigPin, LOW);
  delay(2);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);
  duration = pulseIn(EchoPin, HIGH);
  distance = ((float)(340*duration)/1000)/2;
  delay(5);
}


void go_motor() {
  motor_L.setSpeed(Lspeed);
  motor_R.setSpeed(Rspeed);
  motor_L.run(FORWARD);
  motor_R.run(FORWARD);
}

void back_motor() {
  motor_L.setSpeed(Lspeed);
  motor_R.setSpeed(Rspeed);
  motor_L.run(BACKWARD);
  motor_R.run(BACKWARD);
}

void right_motor() {
  motor_L.setSpeed(Lspeed);
  motor_R.setSpeed(Rspeed);
  motor_L.run(FORWARD);
  motor_R.run(BACKWARD);
}

void left_motor() {
  motor_L.setSpeed(Lspeed);
  motor_R.setSpeed(Rspeed);
  motor_L.run(BACKWARD);
  motor_R.run(FORWARD);
}

void stop_motor() {
  motor_L.run(RELEASE);
  motor_R.run(RELEASE);
}

