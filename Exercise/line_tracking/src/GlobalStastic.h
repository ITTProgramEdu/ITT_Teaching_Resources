#ifndef __GLOBALSTASTIC__
#define __GLOBALSTASTIC__

#define Pi 3.1415926

//data about robot
#define WEEL_DIA 6.5
#define ENCODER_NUM 13.0
#define WEEL_DIS 15.0
#define GEAR_RATIO 34.0

//data about time
#define DEL_TIME2 0.02  //delta time about motor interrupts
#define LOOKUPTIME 10    //1 is equal to 50ms ,a countdown which enables the action to ignore sensor judge
#define UPPER 1000      //1 is equal to 50ms ,a countdown enabling straight action to have a more smooth adjust depending time 

//Set about Pins
#define TimeCLK 11
#define TimeDIO 12

//Set about system
#define total 10        //total num of paths

//Set about Motor PID
#define Proportion 0.7
#define Intergral 0.001
#define Differential 0.000

//Set about Sensor
#define PATH_COLOR 0    //1-black 0-wight

#define Small_ang 0.12



#endif
