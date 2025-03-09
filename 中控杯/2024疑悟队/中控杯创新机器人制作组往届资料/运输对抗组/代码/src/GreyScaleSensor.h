#ifndef GREYSCALESENSOR_H
#define GREYSCALESENSOR_H
#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

class GREYSCALESENSOR {
    public:
        GREYSCALESENSOR(int left, int middle, int right, int compare, int mode);
        int detect();
        int read_sensorL();
        int read_sensorM();
        int read_sensorR();
        int judgeR();
        int judgeL();
        int judgeM();
    private:
        int LPIN;
        int MPIN;
        int RPIN;
        int LD;
        int MD;
        int RD;
        int CMP;
        int MODE;
};
#endif