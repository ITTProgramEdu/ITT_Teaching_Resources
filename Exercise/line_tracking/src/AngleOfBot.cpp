#include "AngleOfBot.h"
#include "Arduino.h"
#include "GlobalStastic.h"
using namespace std;
#define Temp_Const 1000.0
double Current_Angle=0;
void Update_Angle(int Encoder_Delta)
{
  Current_Angle+=((Pi*WEEL_DIA*(double)Encoder_Delta/(GEAR_RATIO*ENCODER_NUM))/WEEL_DIS*Temp_Const);
  if(Current_Angle>2*Pi*Temp_Const)Current_Angle-=2*Pi*Temp_Const;
  if(Current_Angle<-2*Pi*Temp_Const)Current_Angle+=2*Pi*Temp_Const;
}
double Get_Current_Angle()
{
  return Current_Angle/Temp_Const;
}
void Adjust_Angle(double Cur_Angle)
{
  Current_Angle=Cur_Angle*Temp_Const;
}
