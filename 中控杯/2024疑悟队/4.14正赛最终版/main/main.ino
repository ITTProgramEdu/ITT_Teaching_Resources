/**
 * 可用函数
 *  void Start_Set_Velocity();//开始检测速度并控制电机，这个我已经在按钮部分写好，一般不需要另外使用，请见void Start_Button()函数
 *  void Set_Motor_Speed(double Motor1,double Motor2);//设置电机速度,关于前进方向，电机1对应左轮，电机2对应右轮，这个需要常用，例子请见MainLoop子线程
 *  void Stop_Motor();//关闭电机，也在按钮函数部分使用，一般不需要另外用到
 *  
 *  void Raise_Bulldozer_Or_Not(int left,int right);//控制推子的舵机，传入值只有0和1，代表升起和放下推子
 *  void Raise_Arm_Or_Not(int sign);//控制机械臂舵机，具体还没写
 *  void Crawl_Clip_Or_Not(int sign);//控制夹子舵机，具体还没写
 *  
 *  unsigned int Get_Inspect_Result();//获取当前传感器状态，返回值为正数，用位运算可以获取具体某个传感器的状态0/1：
 *                                              bool Sensor_i_state=ret and (1<<(Sensor_Number-i-1));
 *                                    //不过我建议直接通过对应的所有传感器状态对应的数字直接枚举不同情况，让电机做出应对
 *                              
 */

#include "TM1637.h"
#include "GetVelocity_MotorCtl.h"
#include "PIDcontrol.h"
#include "SCoop.h"
#include "Arms.h"
#include "MHSensor.h"
#include "Pin2Interrupt.h"
#include "Straight.h"
#include "Rotate.h"
#include "AngleOfBot.h"
#include "GlobalStastic.h"
#include "BT.h"
using namespace std;

#define ST 1
#define TR 2
#define FP 3
#define WA 4
#define BR 5
#define BL 6
#define BB 7
#define AR 8
#define CL 9
#define SA 10
#define ZZ 11
#define GO 12

TM1637 Display(TimeCLK,TimeDIO);
int Path_Mode=0;                  //0~F十六条路线应该够了
int StartOrNot=0;                  //0未启动 1正在运行
int time_flag=0;
int time_wait;
int time_accu;
unsigned int inspect;
double current_angle;
//unsigned int Sensor_State;        //暂存传感器状态，位运算标识

// int Action_List[16][50] =     {1 ,  1, 3,  2,  3,   1,  1,   3,   2,  3,    1,  1,  1,  1,  1,  1,    1,  3,  2,  3,    1,  1,  1,  1,  1,  3,  2,  3,  1,  1,  1,  3,  2,   3,  1};  //用来储存规划的路线
// double Action_Speed[16][50] = {40,-10,10, 10,  10,   40,-10,  10, -10, 10,  40, 40, 40, 40, 40, 40,   -10, 10, 10, 10,   40, 40, 40, 40, -10,10, 10, 10, 40, 40, -10, 10,-10, 10, 40};
//第0位舍弃不管,后续每个节点代表一个动作
//                                 1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58.  59,  60,  61,  62,  63,  64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79
// char Action_List[10][80] =     {
//                                {  ST,  ST,  FP,  BB,  TR,  ST,  FP,  TR,  ST,  ST,  FP,  TR,  ST,  SA,  BB,  ZZ,  ST,  ST,  FP,  BB,  TR,  ST,  ST,  FP,  TR,  ST,  FP,  TR,  ST,  ST,  FP,  TR,  ST,  ST,  SA},
//                                {  ST,  ST,  FP,  BB,  TR,  ST,  FP,  TR,  ST,  FP,  TR,  ST,  ST,  ST,  FP,  TR,  ST,  ST,  ST,  FP,  TR,  ST,  ST,  SA,  BB},
//                                {  ST,  ST,  FP,  BB,  TR,  ST,  ST,  FP,  TR,  ST,  ST,  FP,  TR,  ST,  ST,  SA,  BB,  SA,  ST,  ST,  FP,  BB,  TR,  ST,  ST,  ST,  ST,  FP,  TR,  ST,  FP,  TR,  ST,  ST,  FP,  TR,  ST,  ST,  SA,  BB},
//                                {  ST,  ST,  FP,  BB,  TR,  ST,  FP,  TR,  ST,  ST,  ST,  ST,  ST,  FP,  TR,  ST,  FP,  TR,  ST,  TR,  ST,  ST,  GO,  BB,  ST,  ST,  FP,  BB,  TR,  ST,  ST,  FP,  TR,  ST,  FP,  TR,  ST,  ST,  FP,  TR,  ST,  ST,  GO,  BB,  ST,  ST,  FP,  BB,  TR,  ST,  ST,  ST,  ST,  FP,  TR,  ST,  FP,  TR,  ST,  ST,  FP,  TR,  ST,  ST,  GO,  BB,  ST,  ST},//A2 2+2+2
//                                {  ST,  ST,  FP,  TR,  ST,  FP,  TR,  ST,  FP,  TR,  ST,  ST,  ST,  FP,  TR,  BR,  ST,  ST,  FP,  TR,  ST,  BR,  ST,  BB,  FP,  TR,  ST,  FP,  TR,  ST,  FP,  TR,  ST,  ST,  FP,  BL,  TR,  BL,  ST,  GO,  BB,  ST,  ST,  FP,  TR,  ST,  ST,  ST,  FP,  TR,  ST,  ST,  FP,  TR,  ST,  ST,  FP,  TR,  ST,  FP,  TR,  ST,  BB,  FP,  TR,  ST,  FP,  TR,  ST,  ST,  FP,  BR,  TR,  BR,  ST,  GO,  BB,  ST,  ST}// 夹子
//                                };  //用来储存规划的路线
// double Action_Speed[10][80] = {{  35,  40,  10,   0, -15,  35,  10,  15,  40,  45,  10,  15,  25, Pi/3,160,  -7, -10, -15,  10,   0,  15,  35,  40,  10,  15,  30,  10,  15,  25,  30,  10,  15,  25,  30,Pi/3},
//                                {  35,  40,  10,   0, -15,  35,  10,  15,  35,  10,  15,  25,  30,  30,  10, -15,  25,  30,  30,  10,  15,  25,  30,Pi/3, 160},
//                                {  35,  40,  10,   0, -15,  35,  40,  10,  15,  25,  30,  10,  15,  25,  30,Pi/4, 160,-Pi/4,-10, -15,  10,   0, -15,  40,  50,  50,  40, 10,  -15,  30,  10, -15,  30,  30,  10, -15,  30,  30,-Pi/2,160},
//                                {  35,  40,  10,   0, -15,  35,  10,  15,  35,  40,  45,  50,  45,  10, -15,  25,  10, -15,  25,  -15, 25,  30, 2e3, 160, -15, -20,  10,   0, -15,  35,  40,  10, -15,  30,  10, -15,  20,  25,  10, -15,  25,  30, 2e3, 160, -15, -20,  10,   0,  15,  35,  40,  45,  40,  10,  15,  35,  10,  15,  25,  30,  10,  15,  25,  30, 2e3, 160, -15, -20},
//                                {  35,  40,  10, -15,  35,  10, -15,  35,  FP,  15,  35,  45,  40,  10,  15,   0,  35,  40,  10,  15,  35, 160,  40,   0,  10,  15,  35,  10, -15,  30,  10, -15,  25,  30,  10, 160,  15,   0,  30, 2e3, 160, -15, -20,  10, -15,  35,  40,  40,  10, -15,  35,  40,  10,  15,  35,  35,  10,  15,  30,  10,  15,  30,   0,  10, -15,  25,  10,  15,  25,  30,  10, 160, -15,   0,  30, 2e3, 160, -15, -20}// 夹子队针对
//                                };
// int List_num[10]={35,25,40,68,79};
//                                  1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58.  59,  60,  61,  62,  63,  64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,  80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95,  96,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126

char Action_List[10][126] =    {{  BB,  ST,  CL,  AR,  CL,  AR,  ST,  GO,  CL},
                                // 夹子 出门两格
                                {  ST,  ST,  FP,  TR,  ST,  FP,  TR,  ST,  FP,  TR,  ST,  ST,  ST,  FP,  TR,  BR,  ST,  ST,  FP,  TR,  ST,  BR,  ST,  BB,  FP,  TR,  ST,  FP,  TR,  ST,  FP,  TR,  ST,  ST,  FP,  BL,  TR,  BL,  ST,  GO,  BB,  ST,  ST,  FP,  TR,  ST,  ST,  ST,  FP,  TR,  ST,  ST,  FP,  TR,  ST,  ST,  FP,  TR,  ST,  FP,  TR,  ST,  BB,  FP,  TR,  ST,  FP,  TR,  ST,  ST,  FP,  BR,  TR,  BR,  ST,  GO,  BB,  ST,  ST,  FP,  TR,  ST,  ST,  FP,  TR,  ST,  ST,  ST,  ST,  FP,  TR,  ST,  ST,  FP,  TR,  ST,  BB,  GO,  SA,  ST,  ST,  FP,  BL,  TR,  BL,  ST,  ST,  ST,  FP,  TR,  ST,  ST,  ST,  ST,  FP,  BR,  TR,  BR,  ST,  ST,  ST,  BL,  TR,  BL,  ST,  GO},
                                //夹子，出门一格
                                {  ST,  FP,  TR,  ST,  ST,  ST,  ST,  FP,  TR,  BR,  ST,  ST,  FP,  TR,  ST,  BR,  ST,  BB,  FP,  TR,  ST,  FP,  TR,  ST,  FP,  TR,  ST,  ST,  FP,  BL,  TR,  BL,  ST,  GO,  BB,  ST,  ST,  FP,  TR,  ST,  ST,  ST,  FP,  TR,  ST,  ST,  FP,  TR,  ST,  ST,  FP,  TR,  ST,  FP,  TR,  ST,  BB,  FP,  TR,  ST,  FP,  TR,  ST,  ST,  FP,  BR,  TR,  BR,  ST,  GO,  BB,  ST,  ST,  FP,  TR,  ST,  ST,  FP,  TR,  ST,  ST,  ST,  ST,  FP,  TR,  ST,  ST,  FP,  TR,  ST,  BB,  GO,  SA,  ST,  ST,  FP,  BL,  TR,  BL,  ST,  ST,  ST,  FP,  TR,  ST,  ST,  ST,  ST,  FP,  BR,  TR,  BR,  ST,  ST,  ST,  BL,  TR,  BL,  ST,  GO},// 夹子                               
                                //夹子，多走一格，留一分给犹豫
                                {  ST,  FP,  TR,  ST,  ST,  ST,  ST,  ST,  FP,  TR,  BR,  ST,  ST,  FP,  TR,  ST,  ST,  BR,  ST,  BB,  FP,  TR,  ST,  FP,  TR,  ST,  FP,  TR,  ST,  ST,  FP,  BL,  TR,  BL,  ST,  GO,  BB,  ST,  ST,  FP,  TR,  ST,  ST,  ST,  FP,  TR,  ST,  ST,  FP,  TR,  ST,  ST,  FP,  TR,  ST,  FP,  TR,  ST,  BB,  FP,  TR,  ST,  FP,  TR,  ST,  ST,  FP,  BR,  TR,  BR,  ST,  GO,  BB,  ST,  ST,  FP,  TR,  ST,  ST,  FP,  TR,  ST,  ST,  ST,  ST,  FP,  TR,  ST,  ST,  FP,  TR,  ST,  BB,  GO,  SA,  ST,  ST,  FP,  BL,  TR,  BL,  ST,  ST,  ST,  FP,  TR,  ST,  ST,  ST,  ST,  FP,  BR,  TR,  BR,  ST,  ST,  ST,  BL,  TR,  BL,  ST,  GO},// 夹子
                                // 排名赛启动区1 排名赛只取前半段，对抗赛则后续偷家
                                {  ST,  ST,  FP,  BB,  TR,  ST,  FP,  TR,  ST,  ST,  ST,  ST,  FP,  TR,  ST,  GO,  BB,  ST,  ST,  ST,  FP,  BB,  TR,  ST,  ST,  ST,  FP,  TR,  ST,  FP,  TR,  ST,  ST,  ST,  FP,  BR,  TR,  BR,  ST,  GO,  BB,  ST,  ST,  FP,  TR,  ST,  FP,  TR,  ST,  ST,  ST,  ST,  FP,  TR,  ST,  FP,  TR,  ST,  BB,  GO,  SA,  ST,  ST,  FP,  BR,  TR,  BR,  ST,  FP,  TR,  ST,  ST,  ST,  ST,  FP,  TR,  ST,  ST,  ST,  FP,  TR,  ST,  GO,  BB,  ST,  ST,  TR,  ST,  ST,  FP,  BB,  TR,  ST,  FP,  TR,  ST,  ST,  FP,  TR,  ST,  ST,  GO},                              
                                // 排名赛启动区2
                                {  ST,  FP,  TR,  ST,  ST,  FP,  BB,  TR,  ST,  FP,  TR,  ST,  ST,  ST,  FP,  BR,  TR,  BR,  ST,  ST,  FP,  TR,  ST,  ST,  GO,  BB,  ST,  ST,  FP,  TR,  ST,  FP,  BB,  TR,  ST,  ST,  ST,  ST,  FP,  TR,  ST,  ST,  ST,  ST,  FP,  TR,  ST,  ST,  ST,  ST,  FP,  TR,  ST,  FP,  BR,  TR,  BR,  ST,  GO,  BB},
                                // 启动区2偷近家
                                {  ST,  ST,  ST,  FP,  TR,  ST,  ST,  FP,  TR,  ST,  FP,  TR,  ST,  BB,  GO,  SA,  ST,  ST,  ST,  FP,  TR,  ST,  ST,  FP,  TR,  ST,  ST,  ST,  FP,  TR,  ST,  ST,  FP,  TR,  ST,  GO,  BB,  ST,  ST},
                                // 小胖路线
                                {  ST,  ST,  FP,  BB,  TR,  ST,  FP,  TR,  ST,  ST,  ST,  ST,  FP,  TR,  ST,  GO,  BB,  ST,  ST,  FP,  TR,  ST,  FP,  TR,  ST,  ST,  ST,  ST,  FP,  BB,  TR,  ST,  FP,  TR,  ST,  ST,  ST,  ST,  FP,  TR,  ST,  ST,  FP,  TR,  ST,  GO,  BB},                              
                                // 排名A1 破坏圆环 高速双车
                                {  ST,  ST,  FP,  BB,  TR,  ST,  FP,  TR,  ST,  ST,  ST,  ST,  FP,  TR,  ST,  GO,  BB,  ST,  ST,  FP,  TR,  ST,  FP,  TR,  ST,  ST,  FP,  TR,  ST,  ST,  FP,  TR,  ST,  FP,  TR,  ST,  BB,  FP,  TR,  ST,  FP,  TR,  ST,  ST,  FP,  BR,  TR,  BR,  ST,  GO,  BB,  ST,  ST,  FP,  TR,  ST,  ST,  FP,  TR,  ST,  ST,  ST,  ST,  FP,  TR,  ST,  ST,  FP,  TR,  ST,  BB,  GO,  SA,  ST,  ST,  FP,  TR,  ST,  FP,  TR,  ST,  ST,  ST,  ST,  FP,  TR,  ST,  ST,  ST,  FP,  BR,  TR,  BR,  ST,  GO,  BB,  ST,  ST},
                                // A2偷远家
                                {  ST,  FP,  TR,  ST,  ST,  FP,  TR,  ST,  ST,  ST,  ST,  ST,  FP,  TR,  ST,  BB,  GO,  SA,  ST,  ST,  FP,  TR,  ST,  FP,  TR,  ST,  ST,  ST,  ST,  FP,  TR,  ST,  ST,  ST,  FP,  BR,  TR,  BR,  ST,  GO,  BB,  ST,  ST}
                               };  //用来储存规划的路线
double Action_Speed[10][126] = {{   0,  30,   1,   1,  0,  0,  30, 2e3,  1},
                                // 夹子 出门两格
                                {  35,  40,  10, -15,  35,  10, -15,  35,  FP,  15,  35,  45,  40,  10,  15,   0,  35,  40,  10,  15,  35, 160,  40,   0,  10,  15,  35,  10, -15,  30,  10, -15,  25,  30,  10, 160,  15,   0,  30, 2e3, 160, -15, -20,  10, -15,  35,  40,  40,  10, -15,  35,  40,  10,  15,  35,  35,  10,  15,  30,  10,  15,  30,   0,  10, -15,  25,  10,  15,  25,  30,  10, 160, -15,   0,  30, 2e3, 160, -15, -20,  10, -15,  35,  40,  10, -15,  35,  40,  50,  40,  10, -15,  35,  35,  10,  15,  30,   0, 2e3, -Pi,  35,  30,  10, 160,  15,   0,  25,  30,  30,  10, -15,  25,  30,  30,  30,  10, 160, -15,   0,  25,  30,  30, 160,  15,   0,  30, 2e3 },// 夹子队针对
                                //夹子， 出门一格  麦克
                                {  35,  10, -15,  35,  35,  45,  40,  10,  15,   0,  35,  40,  10,  15,  35, 160,  40,   0,  10,  15,  35,  10, -15,  30,  10, -15,  25,  30,  10, 160,  15,   0,  30, 2e3, 160, -15, -20,  10, -15,  35,  40,  40,  10, -15,  35,  40,  10,  15,  35,  35,  10,  15,  30,  10,  15,  30,   0,  10, -15,  25,  10,  15,  25,  30,  10, 160, -15,   0,  30, 2e3, 160, -15, -20,  10, -15,  35,  40,  10, -15,  35,  40,  50,  40,  10, -15,  35,  35,  10,  15,  30,   0, 2e3, -Pi,  35,  30,  10, 160,  15,   0,  25,  30,  30,  10, -15,  25,  30,  30,  30,  10, 160, -15,   0,  25,  30,  30, 160,  15,   0,  30, 2e3 },// 夹子队针对
                                //夹子，多走一格，留一分(夹子)给犹豫
                                {  25,  10, -15,  35,  35,  45,  45,  40,  10,  15,   0,  35,  40,  10,  15,  35,  35, 160,  40,   0,  10,  15,  35,  10, -15,  30,  10, -15,  25,  30,  10, 160,  15,   0,  30, 2e3, 160, -15, -20,  10, -15,  35,  40,  40,  10, -15,  35,  40,  10,  15,  35,  35,  10,  15,  30,  10,  15,  30,   0,  10, -15,  25,  10,  15,  25,  30,  10, 160, -15,   0,  30, 2e3, 160, -15, -20,  10, -15,  35,  40,  10, -15,  35,  40,  50,  40,  10, -15,  35,  35,  10,  15,  30,   0, 2e3, -Pi,  35,  30,  10, 160,  15,   0,  25,  30,  30,  10, -15,  25,  30,  30,  30,  10, 160, -15,   0,  25,  30,  30, 160,  15,   0,  30, 2e3 },// 夹子队针对
                                //  排名赛启动区1                                                                                                                                                                                                                                                                                       
                                {  35,  40,  10,   0, -15,  35,  10,  15,  40,  50,  50,  40,  10,  15,  35, 2e3, 160, -20, -20, -25,  10,   0, -15,  35,  40,  40,  10,  15,  35,  10,  15,  35,  30,  30,  10, 160, -15,   0,  40, 2e3, 160, -20, -20,  10, -15,  35,  10, -15,  40,  50,  50,  40,  10, -15,  35,  10,  15,  35,   0, 2e3,  Pi,  35,  35,  10, 160, -15,   0,  35,  10,  15,  40,  50,  50,  40,  10,  15,  40,  50,  40,  10, -15,  35, 2e3, 160, -20, -20,  15,  35,  40,  10,   0,  15,  35,  10,  15,  35,  40,  10,  15,  35,  40, 2e3},
                                // 排名赛启动区2
                                {  25,  10, -15,  35,  40,  10,   0,  15,  35,  10,  15,  35,  40,  40,  10, 160, -15,   0,  35,  35,  10,  15,  35,  35, 2e3, 160, -20, -20,  10,  15,  35,  10,   0,  15,  40,  50,  50,  40,  10,  15,  40,  50,  50,  40,  10,  15,  35,  40,  40,  35,  10,  15,  35,  10, 160, -15,   0,  35, 3e3, 160},
                                // 启动区2偷近家
                                {  35,  40,  40,  10, -15,  35,  40,  10,  15,  35,  10, -15,  35,   0, 2e3, -Pi,  35,  40,  40,  10,  15,  35,  40,  10, -15,  35,  45,  40,  10, -15,  35,  35,  10,  15,  35, 2e3, 160, -20, -20},
                                // 小胖路线
                                {  35,  40,  10,   0, -15,  35,  10,  15,  40,  50,  50,  40,  10,  15,  35, 2e3, 160, -20, -20,  10, -15,  35,  10, -15,  40,  50,  50,  40,  10,   0,  15,  35,  10,  15,  30,  30,  30,  30,  10,  15,  35,  35,  10, -15,  35, 2e3, 160},
                                // 排名A1 破坏圆环 高速双车
                                {  35,  40,  10,   0, -15,  35,  10,  15,  40,  50,  50,  40,  10,  15,  35, 2e3, 160, -20, -20,  10, -15,  40,  10, -15,  35,  40,  10,  15,  35,  35,  10,  15,  30,  10,  15,  30,   0,  10, -15,  25,  10,  15,  25,  30,  10, 160, -15,   0,  30, 2e3, 160, -15, -20,  10, -15,  35,  40,  10, -15,  35,  40,  50,  40,  10, -15,  35,  35,  10,  15,  30,   0, 2e3,  Pi,  35,  30,  10, -15,  35,  10,  15,  35,  40,  50,  40,  10,  15,  35,  40,  40,  10, 160, -15,   0,  40, 2e3, 160, -20, -20},
                                // A2偷远家
                                {  35,  10, -15,  35,  40,  10,  15,  40,  50,  50,  50,  40,  10, -15,  35,   0, 2e3,  Pi,  35,  40,  10, -15,  35,  10,  15,  40,  50,  50,  40,  10,  15,  35,  40,  40,  10, 160, -15,   0,  35, 2e3, 160, -20, -20}
                               };
int List_num[10]={9,126,120,122,102,60,39,47,98,43};

int flag = 0;        // 用于ACtion_Loop 告知 Main_Loop 单次动作是否完成
int action_mode = 0; // 用于Main_Loop 告知 Action_Loop 执行哪一个动作
double action_speed = 0; //用于Main_Loop 告知 Action_Loop 执行动作的速度
int i;

defineTaskLoop(Clock_50ms_Task)
{
  if(time_flag>0)time_flag--;
  if(time_wait>-1)time_wait--;
  //BT_Tran();
  sleep(50);
}

defineTask(Display_Task);
defineTask(Sensor_Task);
//defineTask(Main_Task);
defineTask(Action_Task)

/*_________________________________________________________________________________*/

void Action_Task::setup()
{

}
void Action_Task::loop()
{
  if(StartOrNot and !flag)
  {
    if(action_mode == ST)
    {
      inspect = Get_Inspect_Result();
      if(white2black(inspect))time_wait=0;
      while(!Straight_Finish(inspect) or time_wait>0)
      {
        current_angle = Get_Current_Angle();
        inspect = Get_Inspect_Result();
        BT_Trans();
        if(!StartOrNot)break;
        Straight_Go1(inspect,action_speed,current_angle);
        sleep(20);
      }
      if(i<List_num[Path_Mode]-1 && Action_List[Path_Mode][i+1]==ST && action_speed*Action_Speed[Path_Mode][i+1]>0);
      else Set_Motor_Speed(0,0);
      flag = 1;
    }

    else if(action_mode == TR)
    {
      inspect = Get_Inspect_Result();
      while(!Rotate_Finish(inspect,action_speed))
      {
        inspect = Get_Inspect_Result();
        BT_Trans();
        if(!StartOrNot) break;
        Rotate_Go(inspect,action_speed);
        sleep(30);
      }
     do
      {
        sleep(30);
        
      }while(!Rotate_Adjust(Get_Current_Angle(),Get_Inspect_Result()));
      

      Set_Motor_Speed(0,0);
      sleep(100);
      flag = 1;
    }

    else if(action_mode == FP)
    {
      while(!Find_Parallel(inspect,action_speed))
      {
        inspect = Get_Inspect_Result();
        BT_Trans();
        if(!StartOrNot) break;
        sleep(30);
      }
      Set_Motor_Speed(0,0);
      flag = 1;
    }

    else if(action_mode == WA)
    {
      sleep(action_speed);
      flag = 1;
    }
    else if(action_mode == BR)
    {
      Raise_Right_Bulldozer(160-action_speed);
      flag=1;
    }

    else if(action_mode == BL)
    {
      Raise_Left_Bulldozer(action_speed);
      flag=1;
    }

    else if(action_mode == BB)
    {
      Raise_Right_Bulldozer(160-action_speed);
      Raise_Left_Bulldozer(action_speed);
      flag = 1;
    }

    else if(action_mode == SA)
    {      
      Adjust_Angle(Get_Current_Angle() - action_speed);
      //BT_Trans();
      //Serial.println(action_speed);
      do
      {
        sleep(30); 
        BT_Trans();
      }while(!Rotate_Adjust(Get_Current_Angle(),Get_Inspect_Result()));
      Set_Motor_Speed(0,0);
      sleep(100);
      
      flag = 1;
    }
    else if(action_mode == ZZ)
    {
      do
      {
        sleep(30);
        BT_Trans();
      }while(!Turn(Get_Current_Angle(),action_speed));
      Set_Motor_Speed(0,0);
      Adjust_Angle(0.0);
      sleep(300);
      flag=1;
    }

    else if(action_mode == GO)
    {
      Set_Motor_Speed(10,10);
      sleep(int(action_speed));
      Set_Motor_Speed(0,0);
      flag = 1;
    }
  }
  sleep(49);
}
/*
void Main_Task::setup()
{
  
}
void Main_Task::loop()
{
  
}*/

/*___________________________________________________________________________________________*/



void Display_Task::setup()//晶体管显示线程
{
  Display.set();
  Display.init();
  Display.start();//晶体管显示初始化
}
void Display_Task::loop()
{
  //Display.point(Get_Current_Angle()>0 ? 1:0);
  //Display.display(0,Path_Mode);  //设定晶体管0号位显示路线编号
  //Display.display(3,(int(Get_Current_Angle()*(Get_Current_Angle()>0 ? 1:-1)/3.14*180)%10));
  //Display.display(2,((int(Get_Current_Angle()*(Get_Current_Angle()>0 ? 1:-1)/3.14*180)%100)/10));
  //Display.display(1,((int(Get_Current_Angle()*(Get_Current_Angle()>0 ? 1:-1)/3.14*180)/100)));
  //Display.display(1,i%16);
  Display.display(0,StartOrNot);
  Display.display(1,Path_Mode); //0未启动 1正在运行
  Display.display(2,i/10);
  Display.display(3,i%10);
  Display.point(1);
  sleep(301);

}

void Sensor_Task::setup()//红外传感器线程，每20ms获取一次灰度值
{
  
}

void Sensor_Task::loop()
{
  MHSensor_Inspect();
  sleep(11);
}

void Next_Button()
{
  if(time_flag)return;
  time_flag=1;
  Path_Mode++;
  if(Path_Mode>15)Path_Mode=0;
}
void Last_Button()
{
  if(time_flag)return;
  time_flag=1;
  Path_Mode--;
  if(Path_Mode<0)Path_Mode=15;
}
void Start_Button()//按钮中断函数
{
  if(time_flag>0)return;
  time_flag=10;
  if(StartOrNot==0)//还未启动,则启动
  {
    StartOrNot=1;
    Stop_Arm();
    Start_Set_Velocity();//开始检查电机状态
    detachInterrupt(4);//禁用其它按键
    detachInterrupt(5);
  }
  else //已经启动，则关闭电机、传感器，等待路线选择按钮的指令
  {
    StartOrNot=0;
    flag=1;
    i=0;
    Stop_Arm();
    Stop_Motor();//终止计时器2号，完全关闭电机
    Stop_Straight();
    Stop_Rotate();
    attachInterrupt(4,Next_Button,FALLING);//设定“下一路线”开关，对应在pin19
    attachInterrupt(5,Last_Button,FALLING);//设定“上一路线”开关，对应在pin18
  }
}

void setup()
{
  Serial.begin(9600);
  Set_Velocity_Pins(20,23,21,22);   //电机速度检测接口
  Set_Motor_Pins(48,46,8,49,47,7);//电机驱动接口
  static int MHSensor[12]={A0,A1,A2,A3,A4,A5,A8,A9,A10,A11,A12,A13};
  Set_MHSensor_Pins(12,MHSensor);          //灰度传感器接口
  Set_Servo_Pins(6,5,4,3);               //伺服电机接口

  pinMode(2,INPUT);                      //按钮初始化
  pinMode(18,INPUT);
  pinMode(19,INPUT);
  attachInterrupt(Get_Interrupt_Of_Pin(2),Start_Button,FALLING);//设定起止开关，对应在pin2
  attachInterrupt(Get_Interrupt_Of_Pin(19),Next_Button,FALLING);//设定“下一路线”开关，对应在pin19
  attachInterrupt(Get_Interrupt_Of_Pin(18),Last_Button,FALLING);//设定“上一路线”开关，对应在pin18

  Stop_Arm();
  mySCoop.start();//多线程开始
}
void loop()
{
  //Serial.println(Get_Inspect_Result() & 31);
  yield();//声明：持续多线程
  i=0;
  if(StartOrNot)
  {
    for(i=0;i<List_num[Path_Mode];i++)
    {
      BT_Update_cur_step(i);
      BT_Update_Cur_PathMode(Path_Mode);
      if(!StartOrNot)break;
      flag = 0; //置0，表示action未完成
      time_wait=LOOKUPTIME;
      action_mode = Action_List[Path_Mode][i];
      action_speed = Action_Speed[Path_Mode][i];
      current_angle = Get_Current_Angle();
      //Serial.println(action_speed);
      while(!flag)    //工作未完成则空转
      {
        if(!StartOrNot)break;
        sleep(53);
      }
      if(i<List_num[Path_Mode] && Action_List[Path_Mode][i+1]!=1)Stop_Straight();
      Stop_Rotate();
    }
    i=0;
    StartOrNot=0;
  }
  sleep(51);
}
