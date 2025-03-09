#include "Straight.h"
#include "GetVelocity_MotorCtl.h"
#include "PIDcontrol.h"
#include "GlobalStastic.h"
#include "AngleOfBot.h"
#include<Arduino.h>

#define straight_rate 0.7//(1.35/(fabs(current_angle) + 1.0) - 0.35)
#define sensor_num 7
#define UPPER 1000
double Big_Angle_rate = 0.0;
double Small_Angle_rate = 0.0;
#define Straight_COUNT 15
double Small_Angle[13] = {-1.05,-1.0,-1.0,-0.7,-0.5,-0.3,0,0.3,0.5,0.7,1.0,1.0,1.05};

#define Small_Angle_rate1 1.5/135.0//0.20/9.0   // butui
#define Small_Angle_rate2 6.0/135.0  //1.0/9.0  //推东西
#define Small_Angle_rate3 1.2/135.0   //0.5/9.0 //后退
#define Big_Angle_rate1   0.5        // butui
#define Big_Angle_rate2   1.5        // tui
#define Big_Angle_rate3   1.0        // 后退
#define Push_v  32

#define change_rate 30.0

double Light_SUM = 0;
double Light_CNT = 0;
double Light_AVG = 0;
double real_change = 0;

double sum=0;
int ii;
int tcnt=20;
double weight[sensor_num] = {2,1,0.5,0,-0.5,-1,-2};
double normal;

void Stop_Straight()
{
  tcnt = 2*Straight_COUNT;
  sum=0;
  ii=0;
  Light_SUM = 0;
  Light_CNT = 0;
  Light_AVG = 0;
  real_change = 0;
}

void Straight_Go1(unsigned int inspect12,double normal1,double current_angle)
{
  // normal=normal1;
  //normal*=straight_rate;
  if(normal1 >= Push_v)
  {
    Small_Angle_rate = Small_Angle_rate1;
    Big_Angle_rate = Big_Angle_rate1;
  }

  else if(normal1 >= 0 )
  {
    Small_Angle_rate = Small_Angle_rate1;
    Big_Angle_rate = Big_Angle_rate2;
  }

  else
  {
    Small_Angle_rate = Small_Angle_rate3;
    Big_Angle_rate = Big_Angle_rate3;
  }

  unsigned int inspect = inspect12 >> 5;
  unsigned int inspect_back=inspect12 & 31;
  //if(fabs(current_angle) < 2.5*Small_ang && tcnt>0) tcnt--;
  //else if(fabs(current_angle) >= 1*Small_ang && tcnt < Straight_COUNT)tcnt++;
  if(inspect == 1 and inspect_back == 1)  Adjust_Angle(0);
  if(inspect == 4 and inspect_back == 2)  Adjust_Angle(0);
  if(inspect == 8 and inspect_back == 4)  Adjust_Angle(0);
  if(inspect == 16 and inspect_back == 8)  Adjust_Angle(0);
  if(inspect == 64 and inspect_back == 16)  Adjust_Angle(0);

  if(((inspect & 12) && (inspect_back & 6)) || ((inspect & 24) && (inspect_back & 12)) || ((inspect & 8) && (inspect_back & 4)))normal=normal1;
  else normal=normal1*straight_rate;
  if(fabs(current_angle)<Small_ang)
  {
    if(normal1 > 0)
    {
      for(ii=-3;ii<=3;ii++)
      {
        if(inspect&(1<<(3-ii)))
        {
          Light_SUM += ii;
          Light_CNT += 1;
        }
        if(Light_CNT!=0 and Light_CNT < 5) Light_AVG = Light_SUM / Light_CNT;
      }
      //if(!(inspect & 28))  normal *= straight_rate;

      //if (normal1 >=  Push_v)  
      real_change = Small_Angle_rate * Light_AVG*Light_AVG*Light_AVG;  //三次函数
      //else real_change = Small_Angle[int((Light_AVG+3)*2)];
      //real_change = Small_Angle_rate * Light_AVG;                       // 一次函数
      //real_change = Small_Angle_rate * Light_AVG * fabs(Light_AVG);                   // 二次函数
      Set_Motor_Speed((1.0+real_change)*normal,(1.0-real_change)*normal);
      //else           Set_Motor_Speed((1.0-real_change)*normal,(1.0+real_change)*normal);
      Light_SUM = 0;
      Light_CNT = 0;
      Light_AVG = 0;
    }

    if(normal1 < 0)
    {
      for(ii=-2;ii<=2;ii++)
      {
        if(inspect_back&(1<<(2-ii)))
        {
          Light_SUM += ii;
          Light_CNT += 1;
        }
        if(Light_CNT!=0 /*and Light_CNT < 5*/) Light_AVG = Light_SUM / Light_CNT;
      }
      //if(!(inspect & 28))  normal *= straight_rate;

      //if (normal1 >=  Push_v)  
      real_change = Small_Angle_rate * Light_AVG*Light_AVG*Light_AVG;  //三次函数
      //else real_change = Small_Angle[int((Light_AVG+3)*2)];
      //real_change = Small_Angle_rate * Light_AVG;                       // 一次函数
      //real_change = Small_Angle_rate * Light_AVG * fabs(Light_AVG);                   // 二次函数
      Set_Motor_Speed((1.0+real_change)*normal,(1.0-real_change)*normal);
      //else           Set_Motor_Speed((1.0-real_change)*normal,(1.0+real_change)*normal);
      Light_SUM = 0;
      Light_CNT = 0;
      Light_AVG = 0;
    }
  }
    
  else if(current_angle*normal > 0) //将要往右转调整
  {
    if(normal > 0)
    {
      for(ii=0;ii<=6;ii++)
      {
        if(inspect&(1<<(6-ii)))
        {
          Light_SUM += ii;
          Light_CNT += 1;
        }
        if(Light_CNT!=0 and Light_CNT < 5) Light_AVG = Light_SUM / Light_CNT;
      }
      //normal *= straight_rate;
      real_change = Big_Angle_rate/(7.0-Light_AVG)/(7.0-Light_AVG);
      Set_Motor_Speed((1.0+real_change)*normal,(1.0-real_change)*normal);
      //else           Set_Motor_Speed((1.0-real_change)*normal,(1.0+real_change)*normal); 
      Light_SUM = 0;
      Light_CNT = 0;
      Light_AVG = 0;
    }
    
    else  //后退 这时要右转，左边（高位）最不值钱
    {
      //unsigned int inspect_back = inspect12 & 31; //得到后面的传感器序列
      for(ii=0;ii<=4;ii++)
      {
        if(inspect_back&(1<<(4-ii)))
        {
          Light_SUM += ii;
          Light_CNT += 1;
        }
        if(Light_CNT!=0) Light_AVG = Light_SUM / Light_CNT;
      }
      //normal *= straight_rate;
      real_change = Big_Angle_rate/(5.0-Light_AVG)/(5.0-Light_AVG);
      Set_Motor_Speed((1.0+real_change)*normal,(1.0-real_change)*normal);
      //else           Set_Motor_Speed((1.0-real_change)*normal,(1.0+real_change)*normal); 
      Light_SUM = 0;
      Light_CNT = 0;
      Light_AVG = 0;
    }
  }
  else
  {
    if(normal > 0)
    {
      for(ii=0;ii<=6;ii++)
      {
        if(inspect&(1<<ii))
        {
          Light_SUM += ii;
          Light_CNT += 1;
        }
        if(Light_CNT!=0 and Light_CNT < 5) Light_AVG = Light_SUM / Light_CNT;
      }
      //normal *= straight_rate;
      real_change = Big_Angle_rate/(7.0-Light_AVG)/(7.0-Light_AVG);
      Set_Motor_Speed((1.0-real_change)*normal,(1.0+real_change)*normal);
      //else           Set_Motor_Speed((1.0-real_change)*normal,(1.0+real_change)*normal); 
      Light_SUM = 0;
      Light_CNT = 0;
      Light_AVG = 0;
    }
    
    else
    {
      //unsigned int inspect_back = inspect12 & 31; //得到后面的传感器序列
      for(ii=0;ii<=4;ii++)
      {
        if(inspect_back&(1<<ii))
        {
          Light_SUM += ii;
          Light_CNT += 1;
        }
        if(Light_CNT!=0 and Light_CNT < 5) Light_AVG = Light_SUM / Light_CNT;
      }
      //normal *= straight_rate;
      real_change = Big_Angle_rate/(5.0-Light_AVG)/(5.0-Light_AVG);
      Set_Motor_Speed((1.0-real_change)*normal,(1.0+real_change)*normal);
      //else           Set_Motor_Speed((1.0-real_change)*normal,(1.0+real_change)*normal); 
      Light_SUM = 0;
      Light_CNT = 0;
      Light_AVG = 0;
    }
  }
  //Set_Motor_Speed(normal,normal);
}
int flag_straight;
int Straight_Finish(unsigned int inspect12)
{
    unsigned int inspect = inspect12 >> 5;
    switch(inspect)
    {
      case 31:
      case 62:
      case 63:
      case 124:
      case 126:
      case 127:
      {
//        tcnt = Straight_COUNT;
        return 1;
      }
      default: return 0;
    }
    return 0;
}

bool white2black(unsigned int inspect12)
{
  unsigned int inspect = inspect12 >> 5;
  Light_CNT = 0;
  for(ii=0;ii<=6;ii++)
    if(inspect&(1<<(6-ii))) Light_CNT += 1;
  return Light_CNT < 4;
}