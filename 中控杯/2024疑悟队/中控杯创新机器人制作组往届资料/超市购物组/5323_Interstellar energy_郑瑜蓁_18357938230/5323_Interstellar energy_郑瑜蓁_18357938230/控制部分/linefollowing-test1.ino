#define ML1 9
#define ML2 8
#define MR1 11
#define MR2 10
#define LIN1 46
#define LIN2 48
#define RIN3 50
#define RIN4 52
#define white HIGH
#define black LOW
const int fast=50;
const int slow=30;
int number=0;


void left_motor(int status)
//0 for stop, 1 forward, -1 backward, 2 low forward
{
  if(status==0)
  {
    analogWrite(ML1,0);
    analogWrite(ML2,0);
  }
  else if (status==1)
  {
     analogWrite(ML1,0);
     analogWrite(ML2,fast); 
  }
  else if(status==-1)
  {
    analogWrite(ML1,fast);
    analogWrite(ML2,0);
  }
    else if(status==2)
  {
    analogWrite(ML1,slow);
    analogWrite(ML2,0);
  }
}
void right_motor(int status)
//0 for stop, 1 forward, -1 backward
{
  if(status==0)
  {
    analogWrite(MR1,0);
    analogWrite(MR2,0);
  }
  else if (status==1)
  {
     analogWrite(MR1,0);
     analogWrite(MR2,fast); 
  }
  else if(status==-1)
  {
    analogWrite(MR1,fast);
    analogWrite(MR2,0);
  }
  else if(status==2)
  {
    analogWrite(MR1,slow);
    analogWrite(MR2,0);
  }
}
//car level
void stop() {  left_motor(0);  right_motor(0);}
void forward() {  left_motor(1);  right_motor(1);}
void backward() {  left_motor(-1);  right_motor(-1);}
void left() {  left_motor(2);  right_motor(1);}
void right() {  left_motor(1);  right_motor(2);}

void setup() {
  Serial.begin(9600);
  pinMode(ML1,OUTPUT);
  pinMode(ML2,OUTPUT);
  pinMode(MR1,OUTPUT);
  pinMode(MR2,OUTPUT);
  pinMode(LIN1,INPUT);
  pinMode(LIN2,INPUT);
  pinMode(RIN3,INPUT);
  pinMode(RIN4,INPUT);
}


void loop(){
   int lv1=digitalRead(LIN1),lv2=digitalRead(LIN2),rv3=digitalRead(RIN3),rv4=digitalRead(RIN4);
  if(lv1==black&&rv4==black)
  { backward();delay(50);
    stop();delay(950);
    backward();delay(1000);
    number=number+1;
  }
  else if(lv1!=black||rv4!=black){
  if(number!=4){
    if(lv2==white&&rv3==white)
  {
    forward();
  }
  else if(lv2==black&&rv3==black)
  {
    backward();
  }
  else if(lv2==white&&rv3==black)
  {
    left();delay(500);stop();delay(100);
  }
  else if(lv2==black&&rv3==white)
  {
    right();delay(500);stop();delay(100);
  } }
  else if(number==4){
    left_motor(-1);  right_motor(1);delay(1000);
    left_motor(1);  right_motor(-1);delay(50);stop();delay(450);
    number=number+1;
  } }
}
