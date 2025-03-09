#ifndef __PIDCTL__
#define __PIDCTL__
class PID{
 public:
  double P,I,D;
  double cumulate_err;
  double current_err;
  double last_err;

  PID(double kp,double ki,double kd);
  int PID_Control(double In,double Exp,int Cur);
  void Reset_PID();
};
#endif
