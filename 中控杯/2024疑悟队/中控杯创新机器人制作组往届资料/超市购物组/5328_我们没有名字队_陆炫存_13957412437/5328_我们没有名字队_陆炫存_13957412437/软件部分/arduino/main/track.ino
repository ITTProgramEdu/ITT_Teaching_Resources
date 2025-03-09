void TRACK()
{
  if(VAL_B==0&&VAL_C==1)
    RIGHT();
  else if(VAL_B==1&&VAL_C==0)
    LEFT();
  else{
    FORWARD();
  }
}
void FORWARD()
{
  MotorLeft(100,false);
  MotorRight(100,false);   
}
void RIGHT()
{
  MotorLeft(70,false);
  MotorRight(110,false);
}

void LEFT()
{
  MotorLeft(110,false);
  MotorRight(70,false);
}
void stopfor(){
  MotorLeft(0,true);
  MotorRight(0,true);
}






void BACKTRACK()
{
  if(VAL_B==0&&VAL_C==1)
    BACKLEFT();
  else if(VAL_B==1&&VAL_C==0)
    BACKRIGHT();
  else{
    BACK();
  }
}
void BACK()
{
  MotorLeft(100,true);
  MotorRight(100,true);   
}
void BACKRIGHT()
{
  MotorLeft(70,true);
  MotorRight(110,true);
}
void BACKLEFT()
{
  MotorLeft(110,true);
  MotorRight(70,true);
}
