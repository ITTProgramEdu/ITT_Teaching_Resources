#ifndef __ARMS__
#define __ARMS__
void Set_Servo_Pins(int Bulldozer_L,int Bulldozer_R,int Arm,int Clip);
void Raise_Right_Bulldozer(int right);
void Raise_Left_Bulldozer(int left);
void Raise_Arm(int sign);
void Crawl_Clip(int sign);
void Stop_Arm();
#endif
