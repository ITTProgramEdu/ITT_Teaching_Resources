float checkdistance(int i,int j) {
  digitalWrite(i, LOW);
  delayMicroseconds(2);
  digitalWrite(i, HIGH);
  delayMicroseconds(10);
  digitalWrite(i, LOW);
  float distance = pulseIn(j, HIGH) / 58.00;
  delay(10);
  return distance;
}
