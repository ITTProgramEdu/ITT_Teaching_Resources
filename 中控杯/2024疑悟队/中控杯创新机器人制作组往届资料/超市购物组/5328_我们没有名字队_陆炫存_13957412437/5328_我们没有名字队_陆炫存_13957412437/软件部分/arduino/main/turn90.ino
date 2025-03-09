
unsigned long tim1=0;
void turnleft(){
  MotorLeft(110,false);
  MotorRight(110,true);
  if(VAL_E==LOW){
    if(tim1==0)
      tim1=millis();
    if(millis()-tim1>100){
      tim1=0;
      dir=0;
      Serial.print('d');
    }
  }
} 
void turnright(){
  MotorRight(110,false);
  MotorLeft(110,true);
  if(VAL_D==LOW){
    if(tim1==0)
      tim1=millis();
    if(millis()-tim1>100){
      tim1=0;
      dir=0;
      Serial.print('d');
    }
  }
}
