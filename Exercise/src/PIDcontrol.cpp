#include "PIDcontrol.h"
#include<Math.h>
#include<Arduino.h>
using namespace std;
double P,I,D;
double cumulate_err;
double current_err;
double last_err;
double Out;
PID::PID(double kp,double ki,double kd)
{
  this->P=kp;
  this->I=ki;
  this->D=kd;
  this->last_err=0;
  this->current_err=0;
  this->cumulate_err=0;
}
void PID::Reset_PID()
{
  this->last_err=0;
  this->current_err=0;
  this->cumulate_err=0;
}
int PID::PID_Control(double In,double Exp,int Cur)
{
  this->current_err=Exp-In;
  this->cumulate_err+=this->current_err;
  Out=(double)Cur+this->P*this->current_err+this->I*this->cumulate_err+this->D*(this->last_err-this->current_err);
  this->last_err=this->current_err;

  return (int)(Out);
}
