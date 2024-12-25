#include "Arms.h"
#include "Arduino.h"
#include "Servo.h"
using namespace std;
Servo Bull_l,Bull_r,Arm,Clip;
void Stop_Arm()
{
  Bull_l.write(160);
  Bull_r.write(0);
}
void Set_Servo_Pins(int Bulldozer_L,int Bulldozer_R,int arm,int clip)
{
  pinMode(Bulldozer_L,OUTPUT);
  pinMode(Bulldozer_R,OUTPUT);
  pinMode(arm,OUTPUT);
  pinMode(clip,OUTPUT);

  Bull_l.attach(Bulldozer_L);
  Bull_r.attach(Bulldozer_R);
  Arm.attach(arm);
  Clip.attach(clip);
}
void Raise_Left_Bulldozer(int left)
{
  if(left>=0 && left<=160)Bull_l.write(left);
}
void Raise_Right_Bulldozer(int right)
{
  if(right>=0 && right<=160)Bull_r.write(right);
}
void Raise_Arm(int sign)
{
  
}
void Crawl_Clip(int sign)
{
  
}
