#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <Servo.h>
#include <TB6612.h>
#include <GreyScaleSensor.h>
#include <config.h>
#include <PathPlanning.h>
#include <controller.h>

#define MYPIN 27


/* 相关实例初始化 */
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST); //屏幕
Servo servo; //舵机
TB6612 motor = TB6612(AIN1,AIN2,PWMA,BIN1,BIN2,PWMB,STBY); //电机, A为左, B为右
CONTROLLER controller = CONTROLLER(A0, A1, 14);
GREYSCALESENSOR sensor = GREYSCALESENSOR(A13,A14,A15,300,WHITELINE); //550 for white line, 350 for black line
PATHPLANNING path = PATHPLANNING(); //简易的路径规划算法

int LineError;
int MAXstep = 0;
int step = 0;
int leftcnt = 0, rightcnt = 0;
int intersection_flag = 0;

/* servor_init()函数实现让舵机左右转动，打开推土机结构 */
void servor_init() 
{
    servo.attach(30);
    for (int pos = 90; pos <= 120; pos++) { 
        servo.write(pos);
        delay(10);
    }
    for (int pos = 120; pos >= 60; pos--) {
        servo.write(pos);
        delay(10);
    }
    for (int pos = 60; pos <= 90; pos++) { 
        servo.write(pos);
        delay(10);
    }
    servo.detach();
}
/* 绘制 120 * 150 比赛地图 */
void DrawMap()
{
    tft.drawLine(0,0,120,0,ST7735_CYAN);
    tft.drawLine(0,150,120,150,ST7735_CYAN);
    tft.drawLine(0,0,0,150,ST7735_CYAN);
    tft.drawLine(120,0,120,150,ST7735_CYAN);
    for(int y=1;y<=9;y++)
        tft.drawLine(0,y*15,120,y*15,ST7735_WHITE);
    for(int x=1;x<=7;x++)
        tft.drawLine(15*x,0,15*x,150,ST7735_WHITE);
    tft.fillRect(0,3*15,15,4*15+1,ST7735_MAGENTA);
    tft.fillRect(120,3*15,-15,4*15+1,ST7735_MAGENTA);
    for(int t=2;t<=6;t+=2) {
        tft.fillRect(t*15,0,6,12,ST7735_GREEN);
        tft.fillRect(t*15,0,-6,12,ST7735_GREEN);
    }
    for(int t=2;t<=6;t+=2) {
        tft.fillRect(t*15,150,6,-12,ST7735_GREEN);
        tft.fillRect(t*15,150,-6,-12,ST7735_GREEN);
    }
    tft.fillCircle(3*15,5*15,4,ST7735_WHITE);
    tft.fillCircle(5*15,5*15,4,ST7735_WHITE);

    tft.fillCircle(2*15,3*15,4,ST7735_YELLOW);
    tft.fillCircle(3*15,2*15,4,ST7735_YELLOW);
    tft.fillCircle(4*15,3*15,4,ST7735_YELLOW);
    tft.fillCircle(5*15,2*15,4,ST7735_YELLOW);
    tft.fillCircle(6*15,3*15,4,ST7735_YELLOW);

    tft.fillCircle(2*15,7*15,4,ST7735_YELLOW);
    tft.fillCircle(3*15,8*15,4,ST7735_YELLOW);
    tft.fillCircle(4*15,7*15,4,ST7735_YELLOW);
    tft.fillCircle(5*15,8*15,4,ST7735_YELLOW);
    tft.fillCircle(6*15,7*15,4,ST7735_YELLOW);

    tft.fillCircle(4*15,5*15,4,ST7735_YELLOW);
}

void MessageBox(String s)
{
    tft.fillScreen(ST7735_BLACK);
    tft.drawRoundRect(0,0,128,20,10,ST7735_RED);
    tft.setTextSize(1);
    tft.setTextColor(ST7735_RED);
    tft.setCursor(46,6);
    tft.print("WARNING");
    tft.drawRoundRect(0,25,128,110,10,ST7735_RED);
    tft.setCursor(4,30);
    tft.print(s);
    tft.fillRoundRect(0,140,128,20,10,ST7735_RED);
    tft.setTextColor(ST7735_BLACK);
    tft.setCursor(58,146);
    tft.print("OK");
    controller.get();
}

void monitoring()
{
    if(path.route[step]==GOSTRAIGHT) tft.fillScreen(ST7735_GREEN);
    else if(path.route[step]==UNLOAD) tft.fillScreen(ST7735_ORANGE);
    else if(path.route[step]==TURNRIGHT) tft.fillScreen(ST7735_CYAN);
    else if(path.route[step]==TURNLEFT) tft.fillScreen(ST7735_BLUE);
    else tft.fillScreen(ST7735_YELLOW);
    tft.setCursor(58,64);
    tft.setTextColor(ST7735_BLACK);
    tft.setTextSize(2);
    tft.println(step);
    if(path.route[step] == TURNRIGHT)
        tft.println("\nTURNRIGHT");
    else if(path.route[step] == TURNLEFT)
        tft.println("\nTURNLEFT");
    else if(path.route[step] == TURNBACK)
        tft.println("\nTURNBACK");
    else if(path.route[step]==GOSTRAIGHT)
        tft.println("\nGOSTRAIGHT");
    else if(path.route[step]==UNLOAD)
        tft.println("\nUNLOAD");
}

int Page_Default(int button)
{
    if(button == controller.left)
    {
        /*
        point dest1={4,1},dest2={5,2},dest3={5,6},dest4={3,6},dest5={3,4},dest6={3,2};
        path.path_planning(path.depart_right,dest1,true);
        path.path_planning(dest1,dest2,false);
        path.path_planning(dest2,path.dest1,false);
        path.unload();
        point temp1={5,3};
        path.path_planning(temp1,dest3,false);
        path.path_planning(dest3,dest4,false);
        path.path_planning(dest4,path.dest2,false);
        path.unload();
        point temp2={5,5};
        path.path_planning(temp2,dest5,false);
        path.path_planning(dest5,dest6,false);
        path.path_planning(dest6,path.dest1,false);
        MAXstep = path.unload();
        */
        path.route[MAXstep++]=GOSTRAIGHT;
        path.route[MAXstep++]=GOSTRAIGHT;
        path.route[MAXstep++]=GOSTRAIGHT;
        path.route[MAXstep++]=GOSTRAIGHT;
        path.route[MAXstep++]=GOSTRAIGHT;
        path.route[MAXstep++]=TURNRIGHT;
        path.route[MAXstep++]=UNLOAD;

        path.route[MAXstep++]=GOSTRAIGHT;
        path.route[MAXstep++]=GOSTRAIGHT;
        path.route[MAXstep++]=TURNRIGHT;
        path.route[MAXstep++]=GOSTRAIGHT;
        path.route[MAXstep++]=TURNRIGHT;
        path.route[MAXstep++]=GOSTRAIGHT;
        path.route[MAXstep++]=TURNRIGHT;
        path.route[MAXstep++]=GOSTRAIGHT;
        path.route[MAXstep++]=TURNLEFT;
        path.route[MAXstep++]=UNLOAD;

        path.route[MAXstep++]=UNLOAD;
        path.route[MAXstep++]=GOSTRAIGHT;
        path.route[MAXstep++]=GOSTRAIGHT;
        path.route[MAXstep++]=TURNLEFT;
        path.route[MAXstep++]=TURNLEFT;
        path.route[MAXstep++]=TURNLEFT;
        path.route[MAXstep++]=TURNRIGHT;
        path.route[MAXstep++]=GOSTRAIGHT;
        path.route[MAXstep++]=UNLOAD;
    }
    else if(button == controller.right)
    {
        path.route[MAXstep++]=GOSTRAIGHT;
        path.route[MAXstep++]=TURNLEFT;
        path.route[MAXstep++]=GOSTRAIGHT;
        path.route[MAXstep++]=TURNRIGHT;
        path.route[MAXstep++]=GOSTRAIGHT;
        path.route[MAXstep++]=TURNRIGHT;
        path.route[MAXstep++]=GOSTRAIGHT;
        path.route[MAXstep++]=GOSTRAIGHT;
        path.route[MAXstep++]=UNLOAD;


        path.route[MAXstep++]=GOSTRAIGHT;
        path.route[MAXstep++]=GOSTRAIGHT;
        path.route[MAXstep++]=TURNRIGHT;
        path.route[MAXstep++]=GOSTRAIGHT;
        path.route[MAXstep++]=TURNRIGHT;
        path.route[MAXstep++]=GOSTRAIGHT;
        path.route[MAXstep++]=GOSTRAIGHT;
        path.route[MAXstep++]=GOSTRAIGHT;
        path.route[MAXstep++]=UNLOAD;
        
    }
    else if(button == controller.down)
    {
        path.route[MAXstep++]=GOSTRAIGHT;
        path.route[MAXstep++]=TURNRIGHT;
        path.route[MAXstep++]=GOSTRAIGHT;
        path.route[MAXstep++]=GOSTRAIGHT;
        path.route[MAXstep++]=TURNLEFT;
        path.route[MAXstep++]=GOSTRAIGHT;
        path.route[MAXstep++]=TURNRIGHT;
        path.route[MAXstep++]=GOSTRAIGHT;
        path.route[MAXstep++]=UNLOAD;

        
    }
    else if(button == controller.up)
    {
        path.route[MAXstep++]=GOSTRAIGHT;
        path.route[MAXstep++]=GOSTRAIGHT;
        path.route[MAXstep++]=GOSTRAIGHT;
        path.route[MAXstep++]=TURNRIGHT;
        path.route[MAXstep++]=GOSTRAIGHT;
        path.route[MAXstep++]=GOSTRAIGHT;
        path.route[MAXstep++]=GOSTRAIGHT;
        path.route[MAXstep++]=GOSTRAIGHT;
        path.route[MAXstep++]=UNLOAD;
    }
    Page_Running();
    return 0;
}

int Page_Running()
{
    tft.fillScreen(tft.color565(0,102,80));
    //DrawMap();
    //tft.invertDisplay(true);
    return 0;
}

void Page_Debug()
{
    tft.fillScreen(ST7735_BLACK);
    tft.setTextColor(ST7735_GREEN,ST7735_BLACK);
    tft.setTextWrap(false);
    attachInterrupt(5,leftcounter,RISING);
    attachInterrupt(4,rightcounter,RISING);
    leftcnt=rightcnt=0;
    while(1)
    {
        tft.setCursor(0,0);
        tft.print("Left Sensor:     ");
        tft.print(sensor.read_sensorL());
        tft.println(" ");
        tft.print("Middle Sensor:   ");
        tft.print(sensor.read_sensorM());
        tft.println(" ");
        tft.print("Right Sensor:    ");
        tft.print(sensor.read_sensorR());
        tft.println(" ");
        tft.print("Intersection:    ");
        tft.print(digitalRead(MYPIN));//////////////////////////////////////////////////27!!!
        tft.println(" ");
        tft.print("leftcounter: ");
        tft.println(leftcnt);
        tft.print("rightcounter: ");
        tft.println(rightcnt);
        tft.print("controller: ");
        tft.print(analogRead(A0));
        tft.println(" ");
        delay(100);
    }
    detachInterrupt(5);
    detachInterrupt(4);
}

int Page_Confirm()
{
    tft.fillScreen(ST7735_BLACK);
    tft.drawRoundRect(0,0,128,110,10,ST7735_WHITE);
    tft.setCursor(0,5);
    tft.setTextColor(ST7735_WHITE);
    tft.println(" Here is the path you set:");
    for(int i=0; i<MAXstep;i++){
        if(i==8&&MAXstep>10){
            tft.println(" ......");
            i=MAXstep-1;
        }
        tft.print(" ");
        tft.print(i+1);
        tft.print(". ");
        if(path.route[i]==TURNLEFT) 
            tft.println("TURNLEFT");
        else if(path.route[i]==TURNRIGHT)
            tft.println("TURNRIGHT");
        else if(path.route[i]==TURNBACK)
            tft.println("TURNBACK");
        else if(path.route[i]==GOSTRAIGHT)
            tft.println("GOSTRAIGHT");
        else if(path.route[i]==UNLOAD)
            tft.println("UNLOAD");
    }
    tft.fillRoundRect(0,115,128,20,10,ST7735_MAGENTA);
    tft.setCursor(5,121);
    tft.setTextColor(ST7735_BLACK);
    tft.println("Are you sure to run?");
    tft.fillRoundRect(0,140,63,20,10,ST7735_RED);
    tft.fillRoundRect(64,140,64,20,10,ST7735_BLUE);
    tft.setCursor(0,146);
    tft.println("    Yes        No");
    if(controller.get()==controller.click) return Page_Running();
    else return -1;    
}

int Page_Manual()
{
    int button,button_previous=0;
    int x=5,y=-1;
    tft.fillScreen(ST7735_BLACK);
    DrawMap();
    tft.setTextSize(1);
    tft.setCursor(0,153);
    tft.setTextColor(ST7735_RED);
    tft.print("DEPART:");
    tft.setTextColor(ST7735_WHITE);
    tft.print("LEFT");
    tft.setTextColor(ST7735_RED);
    tft.print("/");
    tft.setTextColor(ST7735_GREEN);
    tft.print("RIGHT");
    button=controller.get();
    if(button==controller.left) {
        x=1;
        y=-1;
    }
    else if(button==controller.right) {
        x=5;
        y=-1;
    }
    
    tft.setCursor(0,153);
    tft.setTextColor(ST7735_BLUE,ST7735_BLACK);
    tft.print("UP");
    tft.setTextColor(ST7735_YELLOW,ST7735_BLACK);
    tft.print("DOWN");
    tft.setTextColor(ST7735_WHITE,ST7735_BLACK);
    tft.print("LEFT");
    tft.setTextColor(ST7735_GREEN,ST7735_BLACK);
    tft.print("RIGHT");
    tft.setTextColor(ST7735_RED,ST7735_BLACK);
    tft.print("ENTER");
    tft.fillCircle((x+1)*15,150-(y+1)*15,2,ST7735_RED);
    
    point depart,dest;
    int flag=1;
    while(1)
    {
        button_previous=button;
        button = controller.get();
        if(button == controller.up) y++;
        else if(button == controller.down) y--;
        else if(button == controller.right) x++;
        else if(button == controller.left) x--;
        else if(button == controller.click) {
            if(y<0){
                MessageBox("Please select at least one point!");
                return -1;
            }
            /*
            MessageBox("Please press the RED button to enable auto navigate mode\n(a mode that the car can automatically dirve itself to the nearest destinataion)");
            if(keyboard.get() == RED) {
                depart.x=x;
                depart.y=y;
                point dest1 = {6,3}, dest2 = {6,5};
                if((x!=dest1.x&&y!=dest1.y)||(x!=dest2.x&&y!=dest2.y)) //auto avigate to the destination
                {
                    if(((x-dest1.x)*(x-dest1.x)+(y-dest1.y)*(y-dest1.y))<=((x-dest2.x)*(x-dest2.x)+(y-dest2.y)*(y-dest2.y)))
                    {
                        MAXstep=path.path_planning(depart,dest1,false);
                    }    
                    else
                        MAXstep=path.path_planning(depart,dest2,false);
                }
            }
            */
            return Page_Confirm();
        }
        if(x<0 || x>6 || y<0 || y>8) {
            MessageBox("Invalid Position!");
            return -1;
        }
        else {
            tft.fillCircle((x+1)*15,150-(y+1)*15,2,ST7735_RED);

            if(flag) { //flag 仅仅用作第一个点的判断
                depart.x=x;
                depart.y=y;
                flag=0;
            }
            else if(depart.x!=x||depart.y!=y) {
                dest.x=x;
                dest.y=y;
                path.path_planning(depart,dest,false);
                MAXstep++;
                if(dest.x == 6 && (dest.y == 3 || dest.y == 5)){
                    path.unload();
                    MAXstep++;
                    dest.x--;
                }
                depart=dest;
            }

        }
        delay(80);
    }
}

int Page_Menu()
{
    MAXstep=0;
    step=0;
    path.clear();


    tft.fillScreen(ST7735_BLACK);
    tft.fillRoundRect(0,0,128,20,10,ST7735_RED);
    tft.fillRoundRect(0,25,128,20,10,ST7735_YELLOW);
    tft.fillRoundRect(0,50,128,20,10,ST7735_BLUE);
    tft.fillRoundRect(0,75,128,20,10,ST7735_GREEN);
    tft.fillRoundRect(0,100,128,20,10,ST7735_WHITE);
    tft.fillRoundRect(0,125,128,20,10,tft.color565(220,220,220));
    tft.setTextSize(1);
    tft.setTextColor(ST7735_BLACK);
    tft.setCursor(46,6);
    tft.print("MANUAL");
    tft.setCursor(46,31);
    tft.print("ROUTE1");
    tft.setCursor(46,56);
    tft.print("ROUTE2");
    tft.setCursor(46,81);
    tft.print("ROUTE3");
    tft.setCursor(46,106);
    tft.print("ROUTE4");

    delay(10);
    float voltage = 11.0*5.0*analogRead(A8)/1024.0;
    tft.setCursor(38,131);
    if(voltage >= 11.1) {
        tft.setTextColor(ST7735_WHITE,tft.color565(220,220,220));
        tft.print("BTR:");
        tft.print(voltage);
        tft.print("V   ");
    }
    else {
        tft.setTextColor(ST7735_RED,tft.color565(220,220,220));
        tft.print("BTR:");
        tft.print(voltage);
        tft.print("V   ");
    }

    int button;
    button = controller.get();
    if(button == controller.click) return Page_Manual();
    else return Page_Default(button);
}
/* 左电机码盘计数器 */
void leftcounter()
{
    leftcnt++;
}
/* 右电机码盘计数器 */
void rightcounter()
{
    rightcnt++;
}

void setup()
{
    Serial.begin(9600);
    analogReference(EXTERNAL);
    tft.initR(INITR_BLACKTAB); 
    motor.stop();
    /* 待用户选择路线 */
    while(Page_Menu()==-1); 

    servor_init();
    motor.runleft(80);
    motor.runright(80);
    while(digitalRead(MYPIN)==LINE);
}

void loop()
{
    LineError = sensor.detect();
    /* 通过以下条件判断是否到达路口 */
    if(digitalRead(MYPIN) == LINE && ((millis()-intersection_flag) > 600)) //间隔大于500毫秒才能判断为下一次路口
    {
        if(step>=MAXstep) {
            motor.stop();
            while(1)tft.fillScreen(ST7735_RED);
        }
        else if(path.route[step]==TURNBACK)
        {
            Serial.print("TURNBACK");
            
            monitoring();

            motor.runright(-40);
            motor.runleft(40);
            int flag1=0, flag2=0, flag3=0;
            while(1)
            {
                if(sensor.judgeM() == ENV) flag1=1;
                if(flag1 && sensor.judgeM() == LINE) flag2=1;
                if(flag2 && sensor.judgeM() == ENV) flag3=1;
                if(flag3 && sensor.judgeR() == LINE) break;
            }
            intersection_flag = millis();
        }
        else if(path.route[step]==TURNRIGHT)
        {
            Serial.print("TURNRIGHT");
            
            monitoring();
            
            motor.runright(-40);
            motor.runleft(40);
            int flag=0;
            while(1)
            {
                if(!flag && sensor.judgeM() == ENV) flag=1;
                if(flag && sensor.judgeR() == LINE) break;
            }
            intersection_flag = millis();
        }
        else if(path.route[step]==TURNLEFT)
        {
            Serial.print("\nTURNLEFT");
            
            monitoring();

            motor.runright(40);
            motor.runleft(-40);
            int flag=0;
            while(1)
            {
                if(!flag && sensor.judgeM() == ENV) flag=1;
                if(flag && sensor.judgeL() == LINE) break;
            }
            intersection_flag = millis();
        }
        else if(path.route[step]==GOSTRAIGHT)
        {
            Serial.print("GOSTRAIGHT");
            
            monitoring();
            intersection_flag = millis();
        }
        else if(path.route[step]==UNLOAD)
        {
            Serial.print("UNLOAD");
            intersection_flag = millis();
            monitoring();
            motor.runright(30);
            motor.runleft(30);
            delay(1000);
            motor.runright(-33);
            motor.runleft(-30);
            
            while(digitalRead(MYPIN)==LINE);
            if(step==MAXstep-1) {
                motor.stop();
                tft.fillScreen(ST77XX_RED);
                while(1);
            }
            delay(500);
            motor.runright(-40);
            motor.runleft(40);
            int flag=0;
            while(1)
            {
                if(sensor.judgeM() == ENV) flag=1;
                if(flag && sensor.judgeR() == LINE) break;
            }
            intersection_flag = millis();
        }
        step++;
    }
    else if(LineError == STRAIGHT)
    {
        motor.runright(80);
        motor.runleft(80);
    }
    else if(LineError == RIGHT)
    {
        motor.runright(65);
        motor.runleft(80);
    }
    else if(LineError == EXTRARIGHT)
    {
        motor.runright(20);
        motor.runleft(80);
    }
    else if(LineError == LEFT)
    {
        motor.runright(80);
        motor.runleft(65);
    }
    else if(LineError == EXTRALEFT)
    {
        motor.runright(80);
        motor.runleft(20);
    }
}