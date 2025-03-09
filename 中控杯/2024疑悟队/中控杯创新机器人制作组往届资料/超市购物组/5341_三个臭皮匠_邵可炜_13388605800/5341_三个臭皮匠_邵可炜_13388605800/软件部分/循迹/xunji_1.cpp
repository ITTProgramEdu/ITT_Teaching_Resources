//修改红外，中间两路也在白线外
// int pintCommunication1=52;//获得识别、抓取或者放置信号，识别、抓取、放置中为1，成功为0
// int pintCommunication2=50;//获得前进信号，需要前进为0，原地等待为1
// int pintCommunication3=48;//到达返回信号0，否则为1
// int pintCommunication4=46;//
int pin21=31;//最左边红外
int pin22=33;//左2
int pin23=35;//右2
int pin24=37;//最右边红外
int pwmPin1=2;//左电机-------------------------------减速电机，二轮驱动
int pwmPin2=3;
int pwmPin3=4;//右电机

int pwmPin4=5;
int count=0;
int lastState=1;//状态，在白线还是黑的
int nowState=1;
const int FAST = 250;
const int MID = 200;
const int SLOW = 100;
int headOrientation=0;
int positionFlag=0;


void countLine()
{
 
  if(lastState==1&&nowState==0)
  {
    count++;
  }
  lastState=nowState;
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
    if(val2==HIGH&&val3==HIGH)//正常直走
    {
      analogWrite(pwmPin1,SLOW);
      analogWrite(pwmPin2,0);
      analogWrite(pwmPin3,SLOW);
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

    if(val2==LOW&&val3==LOW)//十字路口
    {
      analogWrite(pwmPin1,SLOW);
      analogWrite(pwmPin2,0);
      analogWrite(pwmPin3,SLOW);
      analogWrite(pwmPin4,0);
    }
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

void reverse(int n)//开倒车--------测试时有问题，四个轮子应该分别驱动控制，不能同一侧前后两个一起
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
    if(val2==HIGH&&val3==HIGH)//正常直线倒车
    {
      analogWrite(pwmPin1,0);
      analogWrite(pwmPin2,SLOW);
      analogWrite(pwmPin3,0);
      analogWrite(pwmPin4,SLOW);

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

   if(val2==LOW&&val3==LOW)//十字路口
    {
      analogWrite(pwmPin1,0);
      analogWrite(pwmPin2,SLOW);
      analogWrite(pwmPin3,0);
      analogWrite(pwmPin4,SLOW);
    }
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

void turnLeft()//先转过一个角度
{
  int last=millis();//------------------------为什么要两个计时相减小于500？？？？
  int now=millis();
  int valCount=digitalRead(pin24);
  int i=0;
  if(valCount==HIGH)
  {
    nowState=1;
  }
  else
  {
    nowState=0;
  }

  while(now-last<500)
  {
    analogWrite(pwmPin1,0);
    analogWrite(pwmPin2,SLOW);
    analogWrite(pwmPin3,SLOW);
    analogWrite(pwmPin4,0);
    
    now=millis();
  }
  while((digitalRead(pin22)==LOW||digitalRead(pin24)==HIGH)&&count<2)
  {
    countLine();
    analogWrite(pwmPin1,0);
    analogWrite(pwmPin2,SLOW);
    analogWrite(pwmPin3,SLOW);
    analogWrite(pwmPin4,0);
    if(valCount==HIGH)
  {
    nowState=1;
  }
  else
  {
    nowState=0;
  }
    
  }
  count=0;
  analogWrite(pwmPin3,0);
  analogWrite(pwmPin4,0);
  analogWrite(pwmPin1,0);
  analogWrite(pwmPin2,0);
  headOrientation=(headOrientation+1)%4;
}



void turnRight()
{
  int last=millis();
  int now=millis();
  while(now-last<500)
  {
    analogWrite(pwmPin1,SLOW);
    analogWrite(pwmPin2,0);
    analogWrite(pwmPin3,0);
    analogWrite(pwmPin4,SLOW);
    
    now=millis();
  }
  while(digitalRead(pin22)==LOW||digitalRead(pin23)==LOW)
  {
    analogWrite(pwmPin1,SLOW);
    analogWrite(pwmPin2,0);
    analogWrite(pwmPin3,0);
    analogWrite(pwmPin4,SLOW);
    
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





void setup() 
{
  // put your setup code here, to run once:
 // pinMode(pintCommunication1,INPUT);//识别模块给的信号
  // pinMode(pintCommunication2,INPUT);
  // pinMode(pintCommunication3,OUTPUT);//可能是给电机信号
  // pinMode(pintCommunication4,OUTPUT);

  pinMode(pin21,INPUT);
  pinMode(pin22,INPUT);
  pinMode(pin23,INPUT);
  pinMode(pin24,INPUT);
  pinMode(pwmPin1,OUTPUT);
  pinMode(pwmPin2,OUTPUT);
  pinMode(pwmPin3,OUTPUT);
  pinMode(pwmPin4,OUTPUT);
   
  Serial.begin (9600);
  forward(1);  //开始出发到红色1位置
 
}

void loop() 
{
  // put your main code here, to run repeatedly:  
  //delay(2000);//根据规则开始需要等待一定时间
  
  //obtain();
  //place();
}
