#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include "KEYBOARD.h"
#include "config.h"

KEYBOARD::KEYBOARD(int OUT)
{
    OUT_PIN = OUT;
}

char KEYBOARD::get()
{
    int button;
    while(1) {
        button = analogRead(OUT_PIN);
        if(button<400) //BLUE
        {
            while(analogRead(OUT_PIN)<900)delay(5);;
            return BLUE;
        }    
        else if(button<500) //WHITE
        {
            while(analogRead(OUT_PIN)<900)delay(5);;
            return WHITE;
        }
        else if(button<600) //YELLOW
        {
            while(analogRead(OUT_PIN)<900)delay(5);;
            return YELLOW;
        }
        else if(button<700) //GREEN
        {
            while(analogRead(OUT_PIN)<900)delay(5);;
            return GREEN;
        }
        else if(button<900) //RED
        {
            while(analogRead(OUT_PIN)<900)delay(5);;
            return RED;
        }
    }
    delay(5);
}
int KEYBOARD::interrupt()
{
    if(analogRead(OUT_PIN)>=900)return 0;
    else return 1;
}