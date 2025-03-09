const int interval = 500;
int trigPin1=22;//前面测距的超声控制端
int echoPin1=24;//前面测距的超声接收端
int pin3=26;
int pin4=28;
int pin5=30;
int pin6=32;
int pintCommunication1=52;//获得识别、抓取或者放置信号，识别、抓取、放置中为1，成功为0
int pintCommunication2=50;//获得前进信号，需要前进为0，原地等待为1
int pintCommunication3=48;//到达返回信号0，否则为1
int pintCommunication4=46;//
int pin21=31;//最左边红外
int pin22=33;//左2
int pin23=35;//右2
int pin24=37;//最右边红外
int pinc=39;//数线
int pwmPin1=2;//左电机-------------------------------减速电机，二轮驱动
int pwmPin2=3;
int pwmPin3=4;//右电机
int pwmPin4=5;
int count=0;
int lastState=0;//状态，在白线还是黑的
int nowState=0;
int lastTime=-interval;
int nowTime=0;
const int FAST = 250;
const int MID = 200;
const int SLOW = 100;
int headOrientation=0;
int currentPosition;
int targetPosition;
int positionFlag=0;
double cm;
long duration;

void countLine()
{
  nowTime=millis();
  int time=nowTime-lastTime;
  if(nowState!=lastState&&time>interval)
    {
      count++;
      lastTime=nowTime;
    }
}


void forward(int n)
{
  int val1=digitalRead(pin21);//------------------------高电平黑色，低电平白色
  int val2=digitalRead(pin22);
  int val3=digitalRead(pin23);
  int val4=digitalRead(pin24);
  int valCount=digitalRead(pin21);
  if(valCount==HIGH)
  {
    nowState=1;
  }
  else
  {
    nowState=0;
  }

  while(val1==HIGH||val4==HIGH||count<n)
  {     
    countLine(); 
    if(val2==LOW&&val3==LOW)//正常直走
    {
      analogWrite(pwmPin1,SLOW);
      analogWrite(pwmPin2,0);
      analogWrite(pwmPin3,80);
      analogWrite(pwmPin4,0);

    }

    if(val2==HIGH&&val3==LOW) //略微左偏
    {
      analogWrite(pwmPin1,SLOW);
      analogWrite(pwmPin2,0);
      analogWrite(pwmPin3,0);
      analogWrite(pwmPin4,0);
    }

    if(val2==LOW&&val3==HIGH)//略微右偏
    {
      analogWrite(pwmPin1,0);
      analogWrite(pwmPin2,0);
      analogWrite(pwmPin3,SLOW);
      analogWrite(pwmPin4,0);
    }

    /*if(val2==LOW&&val3==LOW)//跑出线了
    {
      analogWrite(pwmPin1,SLOW);
      analogWrite(pwmPin2,0);
      analogWrite(pwmPin3,0);
      analogWrite(pwmPin4,SLOW);
    }*/
    valCount=digitalRead(pin21);
    if(valCount==HIGH)
    {
      nowState=1;
    }
    else
    {
      nowState=0;
    } 
    val1=digitalRead(pin21);
    val2=digitalRead(pin22);
    val3=digitalRead(pin23);
    val4=digitalRead(pin24);
  }
  analogWrite(pwmPin3,0);
  analogWrite(pwmPin4,0);
  analogWrite(pwmPin1,0);
  analogWrite(pwmPin2,0);
  count=0;
}

void reverse(int n)//开倒车
{
  int val1=digitalRead(pin21);
  int val2=digitalRead(pin22);
  int val3=digitalRead(pin23);
  int val4=digitalRead(pin24);  
  int valCount=digitalRead(pin21);
  if(valCount==HIGH)
  {
    nowState=1;
  }
  else
  {
    nowState=0;
  }

  while(val1==HIGH||val4==HIGH||count<n)
  {
    countLine();
    if(val2==LOW&&val3==LOW)//正常直线倒车
    {
      analogWrite(pwmPin1,0);
      analogWrite(pwmPin2,SLOW);
      analogWrite(pwmPin3,0);
      analogWrite(pwmPin4,80);

    }

    if(val2==HIGH&&val3==LOW) //略微左偏
    {
      analogWrite(pwmPin1,0);
      analogWrite(pwmPin2,0);
      analogWrite(pwmPin3,0);
      analogWrite(pwmPin4,SLOW);
    }

    if(val2==LOW&&val3==HIGH)//略微右偏
    {
      analogWrite(pwmPin1,0);
      analogWrite(pwmPin2,SLOW);
      analogWrite(pwmPin3,0);
      analogWrite(pwmPin4,0);
    }

    /*if(val2==LOW&&val3==LOW)//跑出线了
    {
      analogWrite(pwmPin1,SLOW);
      analogWrite(pwmPin2,0);
      analogWrite(pwmPin3,0);
      analogWrite(pwmPin4,SLOW);
    }*/
    val1=digitalRead(pin21);
    val2=digitalRead(pin22);
    val3=digitalRead(pin23);
    val4=digitalRead(pin24);
    valCount=digitalRead(pin21);
    if(valCount==HIGH)
    {
      nowState=1;
    }
    else
    {
      nowState=0;
    }
  }
  analogWrite(pwmPin3,0);
  analogWrite(pwmPin4,0);
  analogWrite(pwmPin1,0);
  analogWrite(pwmPin2,0);
  count=0;
}

void turnLeft()
{
  int last=millis();//------------------------为什么要两个计时相减小于500？？？？
  int now=millis();
  while(now-last<500)
  {
    analogWrite(pwmPin3,SLOW);
    analogWrite(pwmPin4,0);
    analogWrite(pwmPin1,0);
    analogWrite(pwmPin2,SLOW);
    now=millis();
  }
  while(digitalRead(pin22)==HIGH||digitalRead(pin23)==HIGH)
  {
    analogWrite(pwmPin3,SLOW);
    analogWrite(pwmPin4,0);
    analogWrite(pwmPin1,0);
    analogWrite(pwmPin2,SLOW);
  }
  analogWrite(pwmPin3,0);
  analogWrite(pwmPin4,0);
  analogWrite(pwmPin1,0);
  analogWrite(pwmPin2,0);
  headOrientation=(headOrientation+1)%4;//----------------这是在干吗？？？？？？？？？？
}

void turnRight()
{
  int last=millis();
  int now=millis();
  while(now-last<500)
  {
    analogWrite(pwmPin3,0);
    analogWrite(pwmPin4,SLOW);
    analogWrite(pwmPin1,SLOW);
    analogWrite(pwmPin2,0);
    now=millis();
  }
  while(digitalRead(pin22)==HIGH||digitalRead(pin23)==HIGH)
  {
    analogWrite(pwmPin3,0);
    analogWrite(pwmPin4,SLOW);
    analogWrite(pwmPin1,SLOW);
    analogWrite(pwmPin2,0);
  }
  analogWrite(pwmPin3,0);
  analogWrite(pwmPin4,0);
  analogWrite(pwmPin1,0);
  analogWrite(pwmPin2,0);
  headOrientation=(headOrientation-1+4)%4;
}

void adjustOrientation(int targetOrientation)
{
  while(headOrientation!=targetOrientation)
  {
    turnLeft();//----------------------------------为什么是左转调整朝向？？？？？？？？？？？
  }
}

void route()
{
  if(currentPosition==1)
  {
    switch(targetPosition)
    {
      case 5:  //-----------------------------------详见点位图位置
      turnRight();
      forward(2);
      turnRight();
      break;

      case 6:
      turnLeft();
      turnLeft();
      forward(4);
      turnRight();
      forward(2);
      turnLeft();
      forward(1);
      turnRight();
      break;

      case 7:
      turnLeft();
      turnLeft();
      forward(3);
      turnRight();
      forward(7);
      turnRight();
      forward(2);
      break;

      case 8:
      forward(4);
      turnLeft();
      forward(3);
      turnLeft();
      turnLeft();
      break;
    }
    
  }

  if(currentPosition==2)
  {
    switch(targetPosition)
    {
      case 5:  
      turnRight();
      turnRight();
      forward(5);
      turnRight();
      forward(2);
      break;

      case 6:
      turnLeft();
      turnLeft();
      forward(3);
      turnRight();
      forward(6);
      turnRight();
      forward(2);
      turnLeft();
      forward(1);
      turnRight();
      break;

      case 7:
      forward(4);
      turnLeft();
      forward(3);
      turnRight();
      turnRight();
      break;

      case 8:
      turnRight();
      forward(2);
      turnRight();     
      break;
    }
  }

  if(currentPosition==3)
  {
    switch(targetPosition)
    {
      case 5:  
      turnRight();
      turnRight();
      forward(3);
      turnRight();
      forward(7);
      turnRight();
      forward(2);
      break;

      case 6:
      forward(4);
      turnLeft();
      forward(3);
      turnRight();
      turnRight();
      break;

      case 7:
      turnRight();
      forward(2);
      turnRight();
      break;

      case 8:
      turnRight();
      turnRight();
      forward(5);
      turnRight();
      forward(2);     
      break;
    }
  }

  if(currentPosition==4)
  {
    switch(targetPosition)
    {
      case 5:  
      forward(4);
      turnLeft();
      forward(3);
      turnRight();
      turnRight();
      break;

      case 6:
      turnRight();
      forward(2);
      turnRight();
      break;

      case 7:
      turnRight();
      turnRight();
      forward(5);
      turnRight();
      forward(2);
      break;

      case 8:
      turnRight();
      turnRight();
      forward(3);
      turnRight(); 
      forward(7);
      turnRight();
      forward(2);    
      break;
    }
  }

  if(currentPosition==5)
  {
    switch(targetPosition)
    {
      case 1:  
      forward(2);
      turnRight();
      forward(2);
      turnRight();
      break;

      case 2:
      turnLeft();
      turnLeft();
      forward(2);
      turnLeft();
      forward(3);
      break;

      case 3:
      forward(3);
      turnRight();
      forward(7);
      turnRight();
      forward(2);
      break;

      case 4:
      forward(3);
      turnRight();
      forward(6);
      turnRight();
      turnRight();     
      break;
    }
  }

  if(currentPosition==6)
  {
    switch(targetPosition)
    {
      case 1:  
      turnRight();
      forward(1);
      turnRight();
      forward(2);
      turnLeft();
      forward(2);
      break;

      case 2:
      turnRight();
      forward(1);
      turnRight();
      forward(2);
      turnLeft();
      forward(6);
      turnLeft();
      forward(1);
      break;

      case 3:
      forward(3);
      turnRight();
      forward(6);
      turnRight();
      turnRight();
      break;

      case 4:
      forward(2);
      turnRight();
      forward(2);
      turnRight();
      break;
    }
  }

  if(currentPosition==7)
  {
    switch(targetPosition)
    {
      case 1:  
      turnRight();
      turnRight();
      forward(2);
      turnLeft();
      forward(7);
      turnLeft();
      forward(1);
      break;

      case 2:
      forward(3);
      turnRight();
      forward(6);
      turnLeft();
      turnLeft();
      break;

      case 3:
      forward(2);
      turnRight();
      forward(2);
      turnRight();
      break;

      case 4:
      turnRight();
      turnRight();
      forward(2);
      turnLeft();
      forward(3);    
      break;
    }
  }

  if(currentPosition==8)
  {
    switch(targetPosition)
    {
      case 1:  
      turnRight();
      turnRight();
      forward(2);
      turnLeft();
      forward(7);
      turnLeft();
      forward(5);
      turnLeft();
      forward(1);
      break;

      case 2:
      forward(2);
      turnRight();
      forward(2);
      turnRight();
      break;

      case 3:
      turnRight();
      turnRight();
      forward(2);
      turnLeft();
      forward(3);
      break;

      case 4:
      turnRight();
      turnRight();
      forward(2);
      turnLeft();
      forward(7);
      turnLeft();
      forward(1);    
      break;
    }
  }
  digitalWrite(pintCommunication3,LOW);
  currentPosition=targetPosition;
}

void obtain()
{
  int valCommunication1=digitalRead(pintCommunication1); //识别中为1，成功为0
  int valCommunication2=digitalRead(pintCommunication2); //需要前进为0，原地等待为1
  digitalWrite(pintCommunication3,HIGH);//到达标志归位
  //获得目的地接口待定
  while(valCommunication1&&valCommunication2)
  {
    valCommunication1=digitalRead(pintCommunication1);
    valCommunication2=digitalRead(pintCommunication2);
  }

  if(valCommunication1==0)
  {
    turnLeft();
    digitalWrite(trigPin1, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPin1, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin1, LOW);
    duration = pulseIn(echoPin1, HIGH);
    cm = (duration/2) / 29.1;//-------------------------这些数字不懂？？？？？？？？？？
    while(cm>5)
    {
      int val2=digitalRead(pin22);
      int val3=digitalRead(pin23);
      if(val2==LOW&&val3==LOW)//正常直走
      {
        analogWrite(pwmPin1,SLOW);
        analogWrite(pwmPin2,0);
        analogWrite(pwmPin3,80);
        analogWrite(pwmPin4,0);

      }

      if(val2==HIGH&&val3==LOW) //略微左偏
      {
        analogWrite(pwmPin1,SLOW);
        analogWrite(pwmPin2,0);
        analogWrite(pwmPin3,0);
        analogWrite(pwmPin4,0);
      }

      if(val2==LOW&&val3==HIGH)//略微右偏
      {
        analogWrite(pwmPin1,0);
        analogWrite(pwmPin2,0);
        analogWrite(pwmPin3,SLOW);
        analogWrite(pwmPin4,0);
      }
      digitalWrite(trigPin1, LOW);
      delayMicroseconds(5);
      digitalWrite(trigPin1, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin1, LOW);
      duration = pulseIn(echoPin1, HIGH);
      cm = (duration/2) / 29.1;
    }
    analogWrite(pwmPin3,0);
    analogWrite(pwmPin4,0);
    analogWrite(pwmPin1,0);
    analogWrite(pwmPin2,0);
    while(valCommunication1)//等待抓取
    {
      valCommunication1=digitalRead(pintCommunication1);
    }
    reverse(1);
    turnRight();
    targetPosition=5; //获得目的地,假设是5号位,需通讯
    forward(2-positionFlag);
    route();
    positionFlag=0;
    return;
  }
  else//---------------------------------------这边不知道在干吗？？？？？？？？？
  {
    if(positionFlag<2)
    {
      forward(1);
      positionFlag++;
      digitalWrite(pintCommunication3,LOW);
      obtain();
    }
    else
    {
      forward(2);
      turnLeft();
      forward(1);
      positionFlag=0;
      currentPosition++;
      if(currentPosition==5)
      {
        currentPosition-=4;
      }
      digitalWrite(pintCommunication3,LOW);
      obtain();
    }
    
  }
}

void place()
{
  int valCommunication1=digitalRead(pintCommunication1); //放置中为1，成功为0
  int valCommunication2=digitalRead(pintCommunication2); //需要前进为0，原地等待为1
  digitalWrite(pintCommunication3,HIGH);//到达标志归位
  //获得目的地接口待定
  while(valCommunication1&&valCommunication2)
  {
    valCommunication1=digitalRead(pintCommunication1);
    valCommunication2=digitalRead(pintCommunication2);
  }

  if(valCommunication1==0)
  {
    targetPosition=1; //获得目的地,假设是5号位
    reverse(positionFlag);
    route();
    positionFlag=0;
    return;
  }
  else
  {
    if(positionFlag<5)
    {
      forward(1);
      positionFlag++;
      digitalWrite(pintCommunication3,LOW);
      place();
    }
    else
    {
      //用于异常处理
      reverse(positionFlag);
      positionFlag=0;
      place();
    }
    
  }
}

void setup() 
{
  // put your setup code here, to run once:

  pinMode(trigPin1,OUTPUT);
  pinMode(echoPin1,INPUT);
  pinMode(pin3,OUTPUT);
  pinMode(pin4,OUTPUT);
  pinMode(pin5,OUTPUT);
  pinMode(pin6,OUTPUT);
  pinMode(pin21,INPUT);
  pinMode(pin22,INPUT);
  pinMode(pin23,INPUT);
  pinMode(pin24,INPUT);
  pinMode(pinc,INPUT);
  pinMode(pintCommunication1,INPUT);
  pinMode(pintCommunication2,INPUT);
  pinMode(pintCommunication3,OUTPUT);
  pinMode(pintCommunication4,OUTPUT);
  pinMode(pwmPin1,OUTPUT);
  pinMode(pwmPin2,OUTPUT);
  pinMode(pwmPin3,OUTPUT);
  pinMode(pwmPin4,OUTPUT);
  digitalWrite(pintCommunication3,HIGH);  
  Serial.begin (9600);
  forward(4);  
  digitalWrite(pintCommunication3,LOW);
}

void loop() 
{
  // put your main code here, to run repeatedly:  
  //delay(2000);//根据规则开始需要等待一定时间
  
  obtain();
  place();
}
