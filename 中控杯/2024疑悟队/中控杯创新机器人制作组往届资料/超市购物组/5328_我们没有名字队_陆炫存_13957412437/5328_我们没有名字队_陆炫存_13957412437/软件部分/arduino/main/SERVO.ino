void SERVO1(int angle)
{
  int i;
  if(angle>N1)
  {
    for(i=N1;i<angle;i++)
    {
      n1.write(i);
      delay(20);
    }
  }
  else
  {
    for(i=N1;i>=angle;i--)
    {
      n1.write(i);  
      delay(20);
    }
  }
  N1=angle;
}
void SERVO2(int angle)
{
  int i;
  if(angle>N2)
  {
    for(i=N2;i<angle;i++)
    {
      n2.write(i);
      delay(20);
    }
  }
  else
  {
    for(i=N2;i>=angle;i--)
    {
      n2.write(i);  
      delay(20);
    }
  }
  N2=angle;
}

void SERVO3(int angle)
{
  int i;
  if(angle>N3)
  {
    for(i=N3;i<angle;i++)
    {
      n3.write(i);
      delay(20);
    }
  }
  else{
    for(i=N3;i>=angle;i--)
    {
      n3.write(i);  
      delay(20);
    }
  }
  N3=angle;
}

void SERVO4(int angle)
{
  int i;
  if(angle>N4)
  {
    for(i=N4;i<angle;i++)
    {
      n4.write(i);
      delay(20);
    }
  }
  else{
    for(i=N4;i>=angle;i--)
    {
      n4.write(i);  
      delay(20);
    }
  }
  N4=angle;
}

void SERVO5(int angle)
{
  int i;
  if(angle>N5)
  {
    for(i=N5;i<angle;i++)
    {
      n5.write(i);
      delay(20);
    }
  }
  else{
    for(i=N5;i>=angle;i--)
    {
      n5.write(i);  
      delay(20);
    }
  }
  N5=angle;
}

void SERVO6(int angle)
{
  int i;
  if(angle>N6)
  {
    for(i=N6;i<angle;i++)
    {
      n6.write(i);
      delay(20);
    }
  }
  else{
    for(i=N6;i>=angle;i--)
    {
      n6.write(i);  
      delay(20);
    }
  }
  N6=angle;
}
