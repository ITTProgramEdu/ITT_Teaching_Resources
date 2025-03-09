#ifndef CONFIG_H
#define CONFIG_H

#define ENV 1
#define LINE 0

/****    "Adafruit_ST7735.h" & tft monitor    ****/
#define TFT_CS        38
#define TFT_RST        -1
#define TFT_DC         40

/****    "TB6612.h" & motor pin configuration    ****/
#define AIN1        11
#define AIN2        12
#define PWMA        13
#define BIN1        9
#define BIN2        8
#define PWMB        7
#define STBY        10

/****    "Keyboard.h" & button color    ****/
#define BLUE        1
#define WHITE       2
#define YELLOW      3
#define GREEN       4
#define RED         5

/****    "GreyScaleSensor.h" & line following    ****/
#define STRAIGHT    1
#define LEFT        2
#define EXTRALEFT   3
#define RIGHT       4
#define EXTRARIGHT  5
#define CROSS       6

#define BLACKLINE   0
#define WHITELINE   1

/****    "PathPlanning.h" & navigatation    ****/
#define TURNLEFT    1
#define TURNRIGHT   2
#define TURNBACK    3
#define GOSTRAIGHT  4
#define UNLOAD      5
#define STOP        6
#define BACK        7

#define FORWARD     8
#define LEFTWARD    9
#define BACKWARD    10
#define RIGHTWARD   11


typedef struct POINT {
    int x; //0<=x<=7
    int y; //0<=y<=8
}point;


#endif