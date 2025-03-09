//修改路线，走到4位置时结束循环
const int interval = 500;
int trigPin1=22;//前面测距的超声控制端
int echoPin1=24;//前面测距的超声接收端
int pin3=26;
int pin4=28;
int pin5=30;
int pin6=32;

int pintCommunication1=52;//获得识别信号，识别中为1，成功为0
int pintCommunication2=50;//获得前进信号，需要前进为0，原地等待为1
int pintCommunication3=48;//到达返回信号0，否则为1
int pintCommunication4=46;//获得放置信号，放置中为1，成功为0
int pintCommunication5=44;//获得抓取信号，抓取中为1，成功为0
int pintCommunication6=42;//释放抓取信号，需要抓取为1，否则为0
int pintCommunication7=40;//释放放置信号，需要放置为1，否则为0

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
int nowposition=0;//用于跳出循环
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
  
  if (valCommunication1==0)
  {
      digitalWrite(pintCommunication6,HIGH);//可以抓取通讯

      int valCommunication5=digitalRead(pintCommunication5); //抓取中为1，成功为0
      while(valCommunication5)//等待抓取
    {
      valCommunication5=digitalRead(pintCommunication5);
    }

      digitalWrite(pintCommunication6,LOW);//抓取成功初始化
      return;
  }

  else //---------------------------------------识别失败前进一格
  {
      if(positionFlag<5)
      {
      forward(1);
      positionFlag++;
      digitalWrite(pintCommunication3,LOW);
      obtain();
      }
      else
      {
        nowposition++;
        if(nowposition<4)
        {
        turnRight();
        forward(4);
        positionFlag=0;
        digitalWrite(pintCommunication3,LOW);
        obtain();
        }
        else{
        forward(1);//到达5位置停下
        while(1){
        analogWrite(pwmPin3,0);
        analogWrite(pwmPin4,0);
        analogWrite(pwmPin1,0);
        analogWrite(pwmPin2,0);
        }
        }
       
        
      }
  }
  
}

void place()
{
  int valCommunication4=digitalRead(pintCommunication4); //放置中为1，成功为0
  int valCommunication2=digitalRead(pintCommunication2); //需要前进为0，原地等待为1
  digitalWrite(pintCommunication3,HIGH);//到达标志归位
  //获得目的地接口待定
  while(valCommunication4&&valCommunication2)
  {
    valCommunication4=digitalRead(pintCommunication4);
    valCommunication2=digitalRead(pintCommunication2);
  }

  if(valCommunication4==0)//放置成功
  {
    forward(1);
    return;
  }
  else
  {
     place();
    
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
  pinMode(pintCommunication1,INPUT);//识别模块给的信号
  pinMode(pintCommunication2,INPUT);
  pinMode(pintCommunication3,OUTPUT);//可能是给电机信号
  pinMode(pintCommunication4,INPUT);
  pinMode(pintCommunication5,INPUT);
  pinMode(pintCommunication6,OUTPUT);//给机械臂信号
  pinMode(pintCommunication7,OUTPUT);
  pinMode(pwmPin1,OUTPUT);
  pinMode(pwmPin2,OUTPUT);
  pinMode(pwmPin3,OUTPUT);
  pinMode(pwmPin4,OUTPUT);
  digitalWrite(pintCommunication3,HIGH);  
  Serial.begin (9600);
  forward(1);  //开始出发到1位置
  turnleft();
  forward(2);
  digitalWrite(pintCommunication3,LOW);
}

void loop() 
{
  // put your main code here, to run repeatedly:  
  //delay(2000);//根据规则开始需要等待一定时间
  
  obtain();
  place();
}
