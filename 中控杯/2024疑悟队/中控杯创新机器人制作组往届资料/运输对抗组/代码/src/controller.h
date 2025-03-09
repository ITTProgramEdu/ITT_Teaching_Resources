#ifndef CONTROLLER_H
#define CONTROLLER_H
#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
/*
此程序为PS2摇杆手柄库。
用户需首先初始化CONTROLLER实例，在参数中依次输入x方向的模拟输入引脚，y方向的模拟输入引脚，垂直按键的数字输入引脚。
用户需调用scan方法将当前摇杆状态存入state变量中。
值得注意的是，scan方法执行结束当且仅当摇杆状态复位到初始默认位置
用户只需先调用scan方法，再将state属性与click/up/down/left/right进行比较即可获得当前状态
*/
class CONTROLLER {
    public:
        CONTROLLER(uint8_t analog_x, uint8_t analog_y, int digital_press);
        void scan();
        int get();
        int state;
        int click;
        int up;
        int down;
        int left;
        int right;
    private:
        uint8_t XPIN;
        uint8_t YPIN;
        int PRESSPIN;
};
#endif