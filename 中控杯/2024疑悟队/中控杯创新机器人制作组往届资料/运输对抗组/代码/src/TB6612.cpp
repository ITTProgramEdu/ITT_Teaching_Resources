#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include "TB6612.h"

TB6612::TB6612(int AIN1, int AIN2, int PWMA, int BIN1, int BIN2, int PWMB, int STBY)
{
    pinMode(AIN1, OUTPUT);
    pinMode(AIN2, OUTPUT);
    pinMode(PWMA, OUTPUT);
    pinMode(BIN1, OUTPUT);
    pinMode(BIN2, OUTPUT);
    pinMode(PWMB, OUTPUT);
    pinMode(STBY, OUTPUT);
    AIN1_PIN = AIN1;
    AIN2_PIN = AIN2;
    PWMA_PIN = PWMA;
    BIN1_PIN = BIN1;
    BIN2_PIN = BIN2;
    PWMB_PIN = PWMB;
    STBY_PIN = STBY;
}

void TB6612::stop()
{
    digitalWrite(STBY_PIN, LOW);
    digitalWrite(AIN1_PIN, LOW);
    digitalWrite(AIN2_PIN, LOW);
    digitalWrite(BIN1_PIN, LOW);
    digitalWrite(BIN2_PIN, LOW);
    analogWrite(PWMA_PIN, 0);
    analogWrite(PWMB_PIN, 0);
}

void TB6612::runleft(int speed)
{
    digitalWrite(STBY_PIN, HIGH);
    if (speed == 0)
    {
        digitalWrite(AIN1_PIN, LOW);
        digitalWrite(AIN2_PIN, LOW);
    }
    else if (speed > 0)
    {
        if(speed > 255) speed = 255;
        digitalWrite(AIN1_PIN, HIGH);
        digitalWrite(AIN2_PIN, LOW);
        analogWrite(PWMA_PIN, speed);
    }
    else
    {
        speed = -speed;
        if(speed > 255) speed = 255;
        digitalWrite(AIN1_PIN, LOW);
        digitalWrite(AIN2_PIN, HIGH);
        analogWrite(PWMA_PIN, speed);
    }
}

void TB6612::runright(int speed)
{
    if (speed == 0)
    {
        digitalWrite(BIN1_PIN, LOW);
        digitalWrite(BIN2_PIN, LOW);
    }
    else if (speed > 0)
    {
        if(speed > 255) speed = 255;
        digitalWrite(BIN1_PIN, HIGH);
        digitalWrite(BIN2_PIN, LOW);
        analogWrite(PWMB_PIN, speed);
    }
    else
    {
        speed = -speed;
        if(speed > 255) speed = 255;
        digitalWrite(BIN1_PIN, LOW);
        digitalWrite(BIN2_PIN, HIGH);
        analogWrite(PWMB_PIN, speed);
    }
}
