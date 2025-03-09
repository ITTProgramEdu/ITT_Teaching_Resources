#include<Servo.h>
#define servo1  2
#define servo2  3   
#define servo3  4    
#define servo4  5    
#define servo5  6 
#define servo6  7
#define OTB  51
#define OTC  52
#define OTD  50
#define OTE  53
#define OTF  47
#define OTG  49
#define EN1  10
#define EN2  11
#define IN1  14
#define IN2  15  
#define IN3  16  
#define IN4  17 
#define TrgPin 8
#define EcoPin 9
Servo n1;
Servo n2;
Servo n3;
Servo n4;
Servo n5;
Servo n6;
void grab();
void SERVO1();
void SERVO2();
void SERVO3();
void SERVO4();
void SERVO5();
void SERVO6();
void STOP();
void FOWARD();
void RIGHT();
void LEFT();
void LEFT1();
void RIGHT1();
void TRACK();
void turnleft90();
void turnright90();
int N1=100;
int N2=140;
int N3=50;
int N4=130;
int N5=0;
int N6=120;
int VAL_D;
int VAL_B;
int VAL_A;
int VAL_C;
int VAL_E;
int VAL_F,VAL_G;
float distance;
void setup() {
Serial.begin(9600);
  n1.attach(servo1);
  n2.attach(servo2);
  n3.attach(servo3);
  n4.attach(servo4);
  n5.attach(servo5);
  n6.attach(servo6);
  SERVO6(N6);
  SERVO5(N5);
  SERVO4(N4);
  SERVO3(N3);
  SERVO2(N2);
  SERVO1(N1);
//  idle();
  pinMode(EN1,OUTPUT);
  pinMode(EN2,OUTPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
  pinMode(OTD, INPUT);
  pinMode(OTB, INPUT);
  pinMode(OTC, INPUT);
  pinMode(OTE, INPUT);
  pinMode(OTF, INPUT);
  pinMode(OTG, INPUT);
  pinMode(TrgPin,OUTPUT);
  pinMode(EcoPin,INPUT);
  distance = 0;
  int tim=millis();
  while(millis()-tim<1000 || (VAL_F && VAL_G)){
    TRACK();
  }
}
int dir=0;
unsigned long tim=0,st=0;//踩到横线之后超过1s才能再次触发
void loop() {
    JIANCE();
    if(Serial.available()){
      char ch=Serial.read();
      if(ch=='1') dir=1;
      if(ch=='2') dir=2;
      if(ch=='3') dir=3;
      if(ch=='l') dir=4;
      if(ch=='u') dir=5;
      if(ch=='r') dir=6;
      if(ch=='L') dir=7;
      if(ch=='D') dir=8;
      if(ch=='R') dir=9;
      if(ch=='d') dir=10;
      if(ch=='b') dir=11;
      if(ch=='h') dir=12;
      if(ch=='H') dir=13;
      Serial.print('g');
    }
    if(dir==0){
      MotorLeft(0,true);
      MotorRight(0,true);
    }
    if(dir==1){
     TRACK();
      if(millis()-tim>500 && VAL_F==1 && VAL_G==1){
        if(st==0) st=millis();
        else if(millis()-st>50){
          st=0;
          tim=millis();
          dir=0;
          Serial.print('d');
        }
      }
    }
    if(dir==2){
      turnright();//在这个函数里转到位置后会把dir改回0
      tim=millis();
    }
    if(dir==3){
      turnleft();
      tim=millis();
    }
    if(dir==4){
      grabupzuo();
      tim=millis();
      dir=0;
      Serial.print('d');
    }
    if(dir==5){
      grabup();
      tim=millis();
      dir=0;
      Serial.print('d');
    }
    if(dir==6){
      grabupyou();
      tim=millis();
      dir=0;
      Serial.print('d');
    }
    if(dir==7){
      grabdownzuo();
      tim=millis();
      dir=0;
      Serial.print('d');
    }
    if(dir==8){
      grabdown();
      tim=millis();
      dir=0;
      Serial.print('d');
    }
    if(dir==9){
      grabdownyou();
      tim=millis();
      dir=0;
      Serial.print('d');
    }
    if(dir==10){
      qiandiu();
      tim=millis();
      dir=0;
      Serial.print('d');
    }
    if(dir==11){
      fuyuan();
      tim=millis();
      dir=0;
      Serial.print('d');
    }
    if(dir==12){
      TRACK();
      if(VAL_D==LOW && VAL_E==LOW){
        tim=millis();
        dir=0;
        Serial.print('d');
      }
    }
    if(dir==13){
     BACKTRACK();
      if(millis()-tim>500 && VAL_F==1 && VAL_G==1){
        if(st==0) st=millis();
        else if(millis()-st>50){
          st=0;
          tim=millis();
          dir=0;
          Serial.print('d');
        }
      }
    }
}
