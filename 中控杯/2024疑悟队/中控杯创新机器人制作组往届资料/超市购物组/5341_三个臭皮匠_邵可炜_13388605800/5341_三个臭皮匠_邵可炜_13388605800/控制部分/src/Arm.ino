/*
    相关库函数:
    1.servo类成员函数
    attach()              设定舵机的接口，。
    write()               用于设定舵机旋转角度的语句，可设定的角度范围是0°到180°。
    writeMicroseconds()　 用于设定舵机PWMA值的语句，直接用微秒作为参数。
    read()                用于读取舵机角度的语句，可理解为读取最后一条write()命令中的值。
    attached()            判断舵机参数是否已发送到舵机所在接口。
    detach()              使舵机与其接口分离，该接口（9或10）可继续被用作PWM接口。
*/
// #include <Servo.h>
// #include<Arduino.h>
// #include <Adafruit_PWMServoDriver.h>
int ServoPltaformPin = 10;//云台电机
int Servo30KgPin = 9; //自下而上依次电机标号
int Servo20KgPin = 8;
int ServoCameraPin = 7;
int ServoCatchPin = 6;//抓手舵机

/*舵机接入PWM引脚定义*/
#define ServoPltaformPin 10
#define Servo30KgPin 9
#define Servo20KgPin 8
#define ServoCameraPin 7
#define ServoCatchPin 6
/*舵机命名*/
Servo ServoPlatform;//云台旋转舵机
Servo Servo30KG;//抬举臂1
Servo Servo20Kg;//
Servo ServoCamera;
Servo ServoCatch;//抓手

/*机械臂位置初始化*/
void setup() 
{
    ServoPlatform.attach(ServoPltaformPin);
    Servo30KG.attach(Servo30KgPin);
    Servo20Kg.attach(Servo20KgPin);
    ServoCamera.attach(ServoCameraPin);
    ServoCatch.attach(ServoCatchPin);
        //初始化5个电机
    ServoPlatform.write(120);// 云台120时于支架左侧
    Servo30KG.write(700);// 
    Servo20Kg.write(140);// 
    ServoCatch.write(120);
};


void Catch_High() 
{
   
   //初始化5个电机
   ServoPlatform.write(40);// 云台120时平行于支架
   Servo30KG.write(30);// 最大写入数据180贴合到云台位置
   Servo20Kg.write(0);// 数值越大，越向上，58为与上一轴平行位置
   ServoCamera.write(70);// 数值越大，向上旋转，70为与上一轴平行位置
   ServoCatch.write(80);// 抓手控制范围：80/完全打开-120/完全抓合
   delay(2000);
};
/*向反方向旋转至物品放入购物车中*/
void PLace_into_bascket()
{
   ServoPlatform.write(40);// 云台120时平行于支架
   Servo30KG.write(30);// 最大写入数据180贴合到云台位置
   Servo20Kg.write(0);// 数值越大，越向上，58为与上一轴平行位置
   ServoCamera.write(70);// 数值越大，向上旋转，70为与上一轴平行位置
   ServoCatch.write(80);// 抓手控制范围：80/完全打开-120/完全抓合
}
//抓取低位物品
void Catch_Low() 
{
   
   //初始化5个电机
   ServoPlatform.write(40);// 云台120时平行于支架
   Servo30KG.write(30);// 最大写入数据180贴合到云台位置
   Servo20Kg.write(0);// 数值越大，越向上，58为与上一轴平行位置
   ServoCamera.write(70);// 数值越大，向上旋转，70为与上一轴平行位置
   ServoCatch.write(80);// 抓手控制范围：80/完全打开-120/完全抓合`
};

//循环为让抓手反复抓取，作为测试项目
void loop() 
{
    ServoCatch.write(120);
    delay(1000);
    ServoCatch.write(80);
    delay(1000);
}