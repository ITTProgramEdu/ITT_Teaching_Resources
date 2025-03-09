#ifndef KEYBOARD_H
#define KEYBOARD_H
#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

class KEYBOARD {
    public:
        KEYBOARD(int OUT);
        char get();
        int interrupt();
    private:
        int OUT_PIN;
};
#endif