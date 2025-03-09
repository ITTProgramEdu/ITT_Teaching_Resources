#include "MHSensor.h"
#include "Arduino.h"
#include "GlobalStastic.h"

unsigned int Result=0;
unsigned int Res;
int* MH;
int Sensor_num=1;
void Set_MHSensor_Pins(int s_num,int* mh)
{
  MH=malloc(s_num*sizeof(int));
  Sensor_num=s_num;
  for(int i=0;i<s_num;i++)
  {
    MH[i]=mh[i];
    pinMode(MH[i],INPUT);
  }
}
void MHSensor_Inspect()
{
  Res=0;
  for(int i=0;i<Sensor_num;i++)
  {
    Res|=((!(digitalRead(MH[i]) xor PATH_COLOR)) << (Sensor_num-i-1));
  }
  Result=Res;
}
unsigned int Get_Inspect_Result()
{
  return Result;
}
