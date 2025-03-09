#ifndef TB6612_H
#define TB6612_H
#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

class TB6612 {
    public:
        TB6612(int AIN1, int AIN2, int PWMA, int BIN1, int BIN2, int PWMB, int STBY);
        void stop();
        void runright(int speed);
        void runleft(int speed);
    private:
        int AIN1_PIN;
        int AIN2_PIN;
        int PWMA_PIN;
        int BIN1_PIN;
        int BIN2_PIN;
        int PWMB_PIN;
        int STBY_PIN;
};
#endif