void JIANCE(){
  VAL_D=digitalRead(OTD);
  VAL_B=digitalRead(OTB);
  VAL_C=digitalRead(OTC);
  VAL_E=digitalRead(OTE);
  VAL_F=!digitalRead(OTF);
  VAL_G=!digitalRead(OTG);
/*  Serial.print(VAL_B);
  Serial.print(' ');
  Serial.print(VAL_C);
  Serial.print(' ');
  Serial.print(VAL_D);
  Serial.print(' ');
  Serial.println(VAL_E);*/
  //1黑0白
  //白线高电平，黑线低电平
  }
