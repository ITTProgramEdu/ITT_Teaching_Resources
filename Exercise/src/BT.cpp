#include "BT.h"
#include<Arduino.h>
#include "GetVelocity_MotorCtl.h"
#include "AngleOfBot.h"
#include "MHSensor.h"
double lv,rv,ang;
int cur_step;
int cur_path;
int sens;

void BT_Update_cur_step(int i)
{
  cur_step=i;
}
void BT_Update_Cur_PathMode(int pm)
{
  cur_path=pm;
}
void BT_Trans()
{
  lv=Ask_Velocity1();
  rv=Ask_Velocity2();
  ang=Get_Current_Angle();
  sens=Get_Inspect_Result();

  // Serial.print("left_velocity: ");
  // Serial.println(lv);
  // Serial.print("right_velocity: ");
  // Serial.println(rv);
  Serial.print("angle: ");
  Serial.println(ang);
  // Serial.print("front_ins");
  // Serial.println(sens>>5);
  // Serial.print("back_ins");
  // Serial.println(sens&31);
  // Serial.print("step: ");
  // Serial.println(cur_step);
  // Serial.print("Mode: ");
  // Serial.println(cur_path);
  // Serial.write("#");
  
  
}
