#include <MsTimer2.h>
#include <Arduino.h>
#include "GetVelocity_MotorCtl.h"
#include "PIDcontrol.h"
#include "Pin2Interrupt.h"
#include "AngleOfBot.h"
#include "GlobalStastic.h"
int Encoder1_A_Pin;
int Encoder1_B_Pin;
int Encoder2_A_Pin;
int Encoder2_B_Pin;
int Encoder1_Cnt=0;
int Encoder2_Cnt=0;

int Motor1_Enable_A_Pin;
int Motor1_Enable_B_Pin;
int Motor1_Enable_PWM_Pin;
int Motor1_PWM_stastic;
double Motor1_Exp_Velocity;
int Motor2_Enable_A_Pin;
int Motor2_Enable_B_Pin;
int Motor2_Enable_PWM_Pin;
int Motor2_PWM_stastic;
double Motor2_Exp_Velocity;
int Motor1_Pause_Cnt=0;
int Motor2_Pause_Cnt=0;

double v1,v2;

PID* pid_For_Motor1=new PID(Proportion,Intergral,Differential);
PID* pid_For_Motor2=new PID(Proportion,Intergral,Differential);

void Set_Motor_Pins(int M1eA,int M1eB,int M1PWM,int M2eA,int M2eB,int M2PWM)
{
  Motor1_Enable_A_Pin=M1eA;
  Motor1_Enable_B_Pin=M1eB;
  Motor1_Enable_PWM_Pin=M1PWM;
  Motor1_PWM_stastic=0;
  
  Motor2_Enable_A_Pin=M2eA;
  Motor2_Enable_B_Pin=M2eB;
  Motor2_Enable_PWM_Pin=M2PWM;
  Motor2_PWM_stastic=0;

  pinMode(Motor1_Enable_A_Pin,OUTPUT);
  pinMode(Motor1_Enable_B_Pin,OUTPUT);
  pinMode(Motor1_Enable_PWM_Pin,OUTPUT);
  pinMode(Motor2_Enable_A_Pin,OUTPUT);
  pinMode(Motor2_Enable_B_Pin,OUTPUT);
  pinMode(Motor2_Enable_PWM_Pin,OUTPUT);
}
void Set_Motor_Speed(double Motor1_v,double Motor2_v)
{
  Motor1_Exp_Velocity=Motor1_v;
  Motor2_Exp_Velocity=Motor2_v;
  if(Motor1_Exp_Velocity*Motor1_v<0)
  {
    pid_For_Motor1->Reset_PID();
    Motor1_PWM_stastic=0;
  }
  if(Motor2_Exp_Velocity*Motor2_v<0)
  {
    pid_For_Motor2->Reset_PID();
    Motor2_PWM_stastic=0;
  }
}

void Motor_Write(int Pin1,int Pin2,int pinmode1,int pinmode2)
{
  digitalWrite(Pin1,pinmode1);
  digitalWrite(Pin2,pinmode2);
}
void Adjust_Motor_PWM(double Motor1_curV,double Motor2_curV)
{
  //Serial.println(Motor1_curV);
  //Serial.println(Motor2_curV);
  if(Motor1_Exp_Velocity==0.0 && Motor1_Pause_Cnt>=10)
  {
    Motor1_Pause_Cnt=0;
    Motor_Write(Motor1_Enable_A_Pin,Motor1_Enable_B_Pin,LOW,LOW);
    digitalWrite(Motor1_Enable_PWM_Pin,LOW);
    Motor1_PWM_stastic=0;
  }
  else 
  {
    Motor1_PWM_stastic=pid_For_Motor1->PID_Control(Motor1_curV,Motor1_Exp_Velocity,Motor1_PWM_stastic);
    if(Motor1_Exp_Velocity>0)//正转
    {
      Motor_Write(Motor1_Enable_A_Pin,Motor1_Enable_B_Pin,LOW,HIGH);     //纠正正转反转调整这个
      if(Motor1_PWM_stastic>255)
      {
        analogWrite(Motor1_Enable_PWM_Pin,255);
      }
      else if(Motor1_PWM_stastic<0)
      {
        analogWrite(Motor1_Enable_PWM_Pin,0);
      }
      else analogWrite(Motor1_Enable_PWM_Pin,Motor1_PWM_stastic);
    }
    else
    {
      Motor_Write(Motor1_Enable_A_Pin,Motor1_Enable_B_Pin,HIGH,LOW);
      if(Motor1_PWM_stastic<-255)
      {
        analogWrite(Motor1_Enable_PWM_Pin,255);
      }
      else if(Motor1_PWM_stastic>0)
      {
        analogWrite(Motor1_Enable_PWM_Pin,0);
      }
      else analogWrite(Motor1_Enable_PWM_Pin,-Motor1_PWM_stastic);
    }
  }
  if(Motor2_Exp_Velocity==0.0 && Motor2_Pause_Cnt>=10)
  {
    Motor2_Pause_Cnt=0;
    Motor_Write(Motor2_Enable_A_Pin,Motor2_Enable_B_Pin,LOW,LOW);
    digitalWrite(Motor2_Enable_PWM_Pin,LOW);
    Motor2_PWM_stastic=0;
  }
  else 
  {
    Motor2_PWM_stastic=pid_For_Motor2->PID_Control(Motor2_curV,Motor2_Exp_Velocity,Motor2_PWM_stastic);
    if(Motor2_Exp_Velocity>0)//正转
    {
      Motor_Write(Motor2_Enable_A_Pin,Motor2_Enable_B_Pin,LOW,HIGH);     //纠正正转反转调整这个
      if(Motor2_PWM_stastic>255)
      {
        analogWrite(Motor2_Enable_PWM_Pin,255);
      }
      else if(Motor2_PWM_stastic<0)
      {
        analogWrite(Motor2_Enable_PWM_Pin,0);
      }
      else analogWrite(Motor2_Enable_PWM_Pin,Motor2_PWM_stastic);
    }
    else
    {
      Motor_Write(Motor2_Enable_A_Pin,Motor2_Enable_B_Pin,HIGH,LOW);
      if(Motor2_PWM_stastic<-255)
      {
        analogWrite(Motor2_Enable_PWM_Pin,255);
      }
      else if(Motor2_PWM_stastic>0)
      {
        analogWrite(Motor2_Enable_PWM_Pin,0);
      }
      else analogWrite(Motor2_Enable_PWM_Pin,-Motor2_PWM_stastic);
    }
  }    
}

double Ask_Velocity1()
{
  return v1;
}
double Ask_Velocity2()
{
  return v2;
}
void Set_Velocity_Pins(int Ec1_a,int Ec1_b,int Ec2_a,int Ec2_b)
{
  Encoder1_A_Pin=Ec1_a;
  Encoder1_B_Pin=Ec1_b;
  Encoder2_A_Pin=Ec2_a;
  Encoder2_B_Pin=Ec2_b;
}

void Start_Set_Velocity()
{
  MsTimer2::set(DEL_TIME2*1000,ReGet);     // 中断设置函数，每 DEL_TIME2=50ms 进入一次中断
  MsTimer2::start();    
  Adjust_Angle(0.0);
  pinMode(Encoder1_A_Pin, INPUT);
  pinMode(Encoder1_B_Pin, INPUT);
  attachInterrupt(Get_Interrupt_Of_Pin(Encoder1_A_Pin), Read_Quadrature1, FALLING);   //EN1_A脚下降沿触发中断 3
  
  pinMode(Encoder2_A_Pin, INPUT);
  pinMode(Encoder2_B_Pin, INPUT);
  attachInterrupt(Get_Interrupt_Of_Pin(Encoder2_A_Pin), Read_Quadrature2, FALLING);   //EN2_A脚下降沿触发中断 2
}
void Stop_Motor()
{
  MsTimer2::stop();
  Set_Motor_Speed(0,0);
  digitalWrite(Motor1_Enable_A_Pin,LOW);
  digitalWrite(Motor1_Enable_B_Pin,LOW);
  digitalWrite(Motor2_Enable_A_Pin,LOW);
  digitalWrite(Motor2_Enable_B_Pin,LOW);
  Encoder1_Cnt=0;
  Encoder2_Cnt=0;
  pid_For_Motor1->Reset_PID();
  pid_For_Motor2->Reset_PID();
  Motor1_PWM_stastic=0;
  Motor2_PWM_stastic=0;
}

void ReGet()
{
  v1=Pi*WEEL_DIA*(double)Encoder1_Cnt/(GEAR_RATIO*ENCODER_NUM*DEL_TIME2);//编码器线数n=13 轮径d=6.5cm 测量时间t=50ms=0.0.05s 则v=(pi*d*n)/(e*t)
  v2=Pi*WEEL_DIA*(double)Encoder2_Cnt/(GEAR_RATIO*ENCODER_NUM*DEL_TIME2);
  Adjust_Motor_PWM(v1,v2);
  Update_Angle(Encoder2_Cnt-Encoder1_Cnt);
  Encoder1_Cnt=0;
  Encoder2_Cnt=0;
  if(Motor1_Exp_Velocity==0.0)Motor1_Pause_Cnt++;
  if(Motor2_Exp_Velocity==0.0)Motor2_Pause_Cnt++;
}

void Read_Quadrature1()    //编码器脉冲计数中断函数
{
  if (digitalRead(Encoder1_A_Pin) == LOW)
  {
    if (digitalRead(Encoder1_B_Pin) == LOW)    // 查询EN_B的电平以确认正转
    { Encoder1_Cnt++; }
    if (digitalRead(Encoder1_B_Pin) == HIGH)     // 查询EN_B的电平以确认反转
    { Encoder1_Cnt--; }
  }
}
void Read_Quadrature2()    //编码器脉冲计数中断函数
{
  if (digitalRead(Encoder2_A_Pin) == LOW)
  {
    if (digitalRead(Encoder2_B_Pin) == LOW)    // 查询EN_B的电平以确认正转
    { Encoder2_Cnt++; }
    if (digitalRead(Encoder2_B_Pin) == HIGH)     // 查询EN_B的电平以确认反转
    { Encoder2_Cnt--; }
  }
}
