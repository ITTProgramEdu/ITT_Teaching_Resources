#include "Rotate.h"
#include<Arduino.h>
#include "AngleOfBot.h"
#include "Straight.h"
#include "GlobalStastic.h"
#include "GetVelocity_MotorCtl.h"

double Coefficient[2][5] = {1,1,-1,-1,  0,1,1,-1,0,  1};//0转前位置偏转向的一侧，偏内。
double otherwheel[2][5]=   {1,1, 1, 1,0.5,0,1, 1,1,0.5};
#define Left_Finish 4
#define Right_Finish 2

int flagg = 0;
int offset = 0;
int flag_ad = 0;
int adjust_direction = -1;
int last_inspect = 0;
//int in_angle_cnt=0;
double v;


bool Turn(unsigned int inspect12,double normal)
{
  last_inspect=inspect12 & 31;
  if((last_inspect & (1<<2)))return 1;
  Set_Motor_Speed(-normal,normal);
  return 0;
}

bool Rotate_Adjust(double current_angle,unsigned int inspect12)
{
  unsigned int inspect = inspect12 >> 5;
  v = 4.0*fabs(current_angle)+15.0;
 
  if(fabs(current_angle)<2*Small_ang)
  {
    return 1;
  }
 
  if(current_angle > 0) //将要往右转调整
  {
    Set_Motor_Speed(0.5*v,-0.5*v);
  }
  else
  {
    Set_Motor_Speed(-0.5*v,0.5*v);
  }
  return 0;
}
void Stop_Rotate()
{
  //in_angle_cnt=0;
  last_inspect=0;
  adjust_direction=-1;
  flagg=1;
  offset=0;
}
bool Rotate_Finish(unsigned int inspect12,double normal)
{
  //Serial.print("flag=");
  //Serial.print(flagg);
  

  unsigned int inspect = inspect12 >> 5;
  unsigned int back_inspect = inspect12 & 31;
  //Serial.print("   inspect=");
  //Serial.println(inspect);
  if(normal>0)
  {
    //if(flagg==0)
    // {
      // if(inspect&(1<<0))
      // {
        // flagg=1;
      // }
      // return 0;
    // }
    // else 
    if(flagg==1)
    {
      if(!(inspect&(1<<0)))
      {
        flagg=2;
        offset=(((inspect & (1<<6))+(inspect & (1<<5))+(inspect & (1<<4))-(inspect & (1<<2))-(inspect & (1<<1))-(inspect & (1<<0)))<0);
        //右减左大于0，右边踩线，偏左，再左转时偏内，因此是0
      }
      else if(inspect==1 || inspect==2 || inspect==3 || inspect==7)
      {
        Adjust_Angle(Get_Current_Angle()-0.5*Pi);
        return 1;
      }
      return 0;
    }
    else if(flagg==2)
    {
      if(inspect&(1<<0))
      {
        flagg=3;
      }
      return 0;
    }
    else if(flagg==3)
    {
      if(!(inspect&(1<<0)))
      {
        flagg=4;
        Adjust_Angle(Get_Current_Angle()-0.5*Pi);
        return 0;
      }
      return 0;
    }
    else if(flagg==4)
    {
      if(!(back_inspect & (1<<4)))
      {
        flagg=1;
        return 1;
      }
    }
    else
    {
      flagg=1;
      return 0;
    }
    
  }
  else 
  {
    // if(flagg==0)
    // {
    //   if(inspect&(1<<6))
    //   {
    //     flagg=1;
    //   }
    //   return 0;
    // }
    // else 
    if(flagg==1)
    {
      if(!(inspect&(1<<6)))
      {
        flagg=2;
        offset=(((inspect & (1<<6))+(inspect & (1<<5))+(inspect & (1<<4))-(inspect & (1<<2))-(inspect & (1<<1))-(inspect & (1<<0)))>0);
      }
      else if(inspect==64 || inspect==32 || inspect==96 || inspect==112)
      {
        Adjust_Angle(Get_Current_Angle()+0.5*Pi);
        return 1;
      }
      return 0;
    }
    else if(flagg==2)
    {
      if(inspect&(1<<6))
      {
        flagg=3;
      }
      return 0;
    }
    else if(flagg==3)
    {
      if(!(inspect&(1<<6)))
      {
        flagg=4;
        Adjust_Angle(Get_Current_Angle()+0.5*Pi);
        return 0;
      }
      return 0;
    }
    else if(flagg==4)
    {
      if(!(back_inspect & (1<<0)))
      {
        flagg=1;
        return 1;
      }
    }
    else
    {
      flagg=1;
      return 0;
    }
    
  }
  


  // Serial.print("bool = ");
  // Serial.println((!(inspect & (1<<0))) and ((inspect & (1<<3)) or (inspect & (1<<4)) or (inspect & (1<<5)) ));
  // Serial.print("inspect = ");
  // Serial.println(inspect);
  // if (normal < 0)
  //   return (!(inspect & (1<<0))) and ((inspect & (1<<3)) or (inspect & (1<<4)) or (inspect & (1<<5)) );
  // else return (!(inspect & (1<<6))) and ((inspect & (1<<3)) or (inspect & (1<<2)) or (inspect & (1<<1)) );
}

void Rotate_Go(unsigned int inspect12, double normal)
{
  unsigned int inspect = inspect12 >> 5;
  if(normal>0)
  {
    Set_Motor_Speed(Coefficient[offset][flagg]*normal,otherwheel[offset][flagg]*normal);
  } 
  else Set_Motor_Speed(-otherwheel[offset][flagg]*normal,-Coefficient[offset][flagg]*normal);
}

bool Find_Parallel(unsigned int inspect12,double normal)
{
  unsigned int inspect = inspect12 >> 5;
  v=(normal>0 ? normal:-normal)*0.5;
  if(inspect == 127 || inspect == 126 || inspect == 63 || inspect == 125 || inspect == 123 || inspect == 119 || inspect == 111 || inspect == 95) 
  {
    adjust_direction=-1;
    last_inspect=0;
    //Adjust_Angle(0.0);//找平后设置当前角度为0
    return 1;
  }
  if(Get_Current_Angle()>Small_ang)
  {
    if( (inspect&(1<<6)) && (!(inspect&(1<<0))) )Set_Motor_Speed(0,-v);
    else if( (!(inspect&(1<<6))) && ((inspect&(1<<0))) )Set_Motor_Speed(v,0);
    else Set_Motor_Speed(-normal,-normal);
  }
  else if(Get_Current_Angle()<-Small_ang)
  {
    if( (!(inspect&(1<<6))) && ((inspect&(1<<0))) )Set_Motor_Speed(-v,0);
    else if( ((inspect&(1<<6))) && (!(inspect&(1<<0))) )Set_Motor_Speed(0,v);
    else Set_Motor_Speed(-normal,-normal);
  }
  else Set_Motor_Speed(-normal,-normal);
  /*
  if(inspect<last_inspect)adjust_direction*=(-1);
  last_inspect=inspect;
  switch(inspect)
  {
    case 63:
    case 31:Set_Motor_Speed(normal*adjust_direction,0);break;
    case 126:
    case 124:Set_Motor_Speed(0,normal*adjust_direction);break;
    default : Set_Motor_Speed(normal*adjust_direction,normal*adjust_direction);break;

  }*/
  return 0;
}
