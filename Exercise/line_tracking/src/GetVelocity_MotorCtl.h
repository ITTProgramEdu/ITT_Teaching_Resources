#ifndef __GETVELOCITY_MOTORCTL__
#define __GETVELOCITY_MOTORCTL__
#define LOW 0
#define HIGH 1

void Set_Velocity_Pins(int Ec1_a,int Ec1_b,int Ec2_a,int Ec2_b);//Pin-A必须接20、21端口，因为使用了mega2560的2、3中断
void Start_Set_Velocity();//开始计时器中断并开始检测速度
void Read_Quadrature1();
void Read_Quadrature2();
void ReGet();
double Ask_Velocity1();//返回当前所指电机速度单位是cm/s
double Ask_Velocity2();

void Set_Motor_Pins(int M1eA,int M1eB,int M1PWM,int M2eA,int M2eB,int M2PWM);
void Set_Motor_Speed(double Motor1,double Motor2);//设置电机速度
void Stop_Motor();//终止电机

void Motor_Write(int Pin1,int Pin2,int pinmode1,int pinmode2);
void Adjust_Motor_PWM(double Motor1_curV,double Motor2_CurV);//PWM自动微调电机,没必要在主函数掉用

#endif
