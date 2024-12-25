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
#define Push_v  32                   // 我们认为的推东西速度临界点

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
  /* 
      用于处理巡线时直走情况的函数

      函数输入inspect12，normal1，current_angle三个变量

      没有输出，将直接使用 Set_Motor_Speed 函数调整车轮速度

      inspect12：0 ~ 4095的一个整数，将其转化为二进制数则对应12个传感器的状态，1代表检测到白色，2代表检测到黑色。其中前7位为小车前方7个传感器，后5位为后方5个传感器。
      normal1：当前的速度，前进为正，后退为负，前进时一般在30~50.
      current_angle：与直线的夹角（即偏离路线的角度），逆时针为正，顺时针为负。单位为弧度制，也即当current_angle=0时，车头正对直线,当current_angle=pi/4时，车头向左偏了45度。

      Set_Motor_Speed(左轮速度，右轮速度)：将左右轮速度调节到合适的速度。例如
      Set_Motor_Speed(normal1,normal1)将使车轮保持原速度不变。
      Set_Motor_Speed(normal1*0.1,normal1*0.1)。速度变为原来的0.1倍。
  */

  unsigned int inspect = inspect12 >> 5;
  unsigned int inspect_back=inspect12 & 31; 
  // 以上两个语句将前后方传感器分离开来
  // 运用位运算 如 & | 等可以方便二进制数的数据处理

  /*----------------在下方编写你的代码---------------------*/





  Set_Motor_Speed(normal1,normal1);  // 此语句需要修改，放置此处起提醒作用
  /*----------------在上方编写你的代码---------------------*/
  return;
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