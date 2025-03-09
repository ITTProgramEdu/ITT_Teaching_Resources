#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include "controller.h"

CONTROLLER::CONTROLLER(uint8_t analog_x, uint8_t analog_y, int digital_press)
{
    XPIN = analog_x;
    YPIN = analog_y;
    PRESSPIN = digital_press;
    pinMode(PRESSPIN, INPUT);
    click = 0;
    up = 1;
    down = 2;
    left = 3;
    right = 4;
}

void CONTROLLER::scan()
{
    int press = digitalRead(PRESSPIN);
    int x = analogRead(XPIN);
    int y = analogRead(YPIN);
    state = -1;
    if(press == LOW)
    {
        while(digitalRead(PRESSPIN) == LOW);
        state = click;
    }
    else
    {
        if(y < 256)
            state = up;
        else if(y > 256*3)
            state = down;
        else if(x < 256)
            state = left;
        else if(x > 256*3)
            state = right;
        while(x<256 || x>256*3 || y<256 || y>256*3)
        {
            x = analogRead(XPIN);
            y = analogRead(YPIN);
        }
    }
}
int CONTROLLER::get()
{
    state = -1;
    while(state == -1)
    {
        scan();
        delay(10);
    }
    return state;
}