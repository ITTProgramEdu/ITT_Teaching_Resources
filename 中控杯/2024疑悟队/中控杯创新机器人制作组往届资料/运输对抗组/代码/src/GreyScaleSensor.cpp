#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include "GreyScaleSensor.h"
#include "config.h"

GREYSCALESENSOR::GREYSCALESENSOR(int left, int middle, int right, int compare, int mode)
{
    LPIN = left;
    MPIN = middle;
    RPIN = right;
    CMP = compare;
    MODE = mode;
}

int GREYSCALESENSOR::detect()
{
    LD = analogRead(LPIN);
    MD = analogRead(MPIN);
    RD = analogRead(RPIN);
    if(MODE == WHITELINE)
    {
        if(LD < CMP && MD >= CMP && RD < CMP)
        {
            return STRAIGHT;
        }
        else if(LD >= CMP && MD >= CMP && RD < CMP)
        {
            return LEFT; 
        }
        else if(LD >= CMP && MD < CMP && RD < CMP)
        {
            return EXTRALEFT; 
        }
        else if(LD < CMP && MD >= CMP && RD >= CMP)
        {
            return RIGHT; 
        }
        else if(LD < CMP && MD < CMP && RD >= CMP)
        {
            return EXTRARIGHT; 
        }
        else if(LD >= CMP && MD >= CMP && RD >= CMP)
        {
            return STRAIGHT; 
        }
        return STRAIGHT;
    }
    else if(MODE == BLACKLINE)
    {
        if(LD > CMP && MD <= CMP && RD > CMP)
        {
            return STRAIGHT;
        }
        else if(LD <= CMP && MD <= CMP && RD > CMP)
        {
            return LEFT; 
        }
        else if(LD <= CMP && MD > CMP && RD > CMP)
        {
            return EXTRALEFT; 
        }
        else if(LD > CMP && MD <= CMP && RD <= CMP)
        {
            return RIGHT; 
        }
        else if(LD > CMP && MD > CMP && RD <= CMP)
        {
            return EXTRARIGHT; 
        }
        else if(LD <= CMP && MD <= CMP && RD <= CMP)
        {
            return STRAIGHT; 
        }
        return STRAIGHT;
    }
}
int GREYSCALESENSOR::read_sensorL()
{
    return analogRead(LPIN);
}
int GREYSCALESENSOR::read_sensorM()
{
    return analogRead(MPIN);
}
int GREYSCALESENSOR::read_sensorR()
{
    return analogRead(RPIN);
}
int GREYSCALESENSOR::judgeR()
{
    if(MODE == WHITELINE)
    {
        if(analogRead(RPIN)<CMP) return ENV;
        else if(analogRead(RPIN)>CMP) return LINE;
    }
    else if(MODE == BLACKLINE)
    {
        if(analogRead(RPIN)<CMP) return LINE;
        else if(analogRead(RPIN)>CMP) return ENV;
    }
}
int GREYSCALESENSOR::judgeL()
{
    if(MODE == WHITELINE)
    {
        if(analogRead(LPIN)<CMP) return ENV;
        else if(analogRead(LPIN)>CMP) return LINE;
    }
    else if(MODE == BLACKLINE)
    {
        if(analogRead(LPIN)<CMP) return LINE;
        else if(analogRead(LPIN)>CMP) return ENV;
    }
}
int GREYSCALESENSOR::judgeM()
{
    if(MODE == WHITELINE)
    {
        if(analogRead(MPIN)<CMP) return ENV;
        else if(analogRead(MPIN)>CMP) return LINE;
    }
    else if(MODE == BLACKLINE)
    {
        if(analogRead(MPIN)<CMP) return LINE;
        else if(analogRead(MPIN)>CMP) return ENV;
    }
}
