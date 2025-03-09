//This motor shield use Pin 6,5,8,7,4,3 to control the motor
// Simply connect your motors to M1+,M1-,M2+,M2-
// Upload the code to Arduino/Roboduino
// Through serial monitor, type 'a','s', 'w','d','x' to control the motor
void MotorLeft(int pwm, boolean reverse)
        {
          analogWrite(EN1,pwm); //set pwm control, 0 for stop, and 255 for maximum speed
         if(reverse)
         { 
          digitalWrite(IN1,HIGH);
          digitalWrite(IN2,LOW);    
         }
        else
        {
          digitalWrite(IN1,LOW);  
          digitalWrite(IN2,HIGH);  
         }
        }  
        
void MotorRight(int pwm, boolean reverse)
        {
          analogWrite(EN2,pwm);
         if(reverse)
         { 
          digitalWrite(IN3,HIGH);  
          digitalWrite(IN4,LOW);     
         }
        else
        {
          digitalWrite(IN3,LOW); 
          digitalWrite(IN4,HIGH);    
         }
        }  
