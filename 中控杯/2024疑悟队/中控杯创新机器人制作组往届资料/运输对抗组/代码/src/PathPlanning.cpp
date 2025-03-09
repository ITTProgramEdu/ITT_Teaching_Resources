#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

#include "PathPlanning.h"
#include "config.h"

PATHPLANNING::PATHPLANNING()
{
    ;
}

void PATHPLANNING::rotate(int initial_direction,int final_direction) //to rotate the car and then go straight, which shall not be used in the main loop
{
    if(initial_direction-final_direction == 2 || initial_direction-final_direction == -2) *proute++ = TURNBACK;
    else if(initial_direction-final_direction == 1 || initial_direction-final_direction == -3) *proute++ = TURNRIGHT;
    else if(initial_direction-final_direction == -1 || initial_direction-final_direction == 3) *proute++ = TURNLEFT;
    else *proute++ = GOSTRAIGHT;
    return;
}
int PATHPLANNING::validate_linepath(point a, point b) //to check whether a line path is validated (without obstacles),  which shall not be used in the main loop
{
    int x=a.x, y=a.y;
    int status = true;
    if(a.x==b.x && a.y<b.y) {
        for(y=a.y;y<=b.y;y++) if(forbidmap[x][y])break;
        if(y<=b.y)status=false;
        return status;
    }
    else if(a.x==b.x && a.y>b.y) {
        for(y=a.y;y>=b.y;y--) if(forbidmap[x][y])break;
        if(y>=b.y)status=false;
        return status;
    }
    else if(a.x<b.x && a.y==b.y) {
        for(x=a.x;x<=b.x;x++) if(forbidmap[x][y])break;
        if(x<=b.x)status=false;
        return status;
    }
    else if(a.x>b.x && a.y==b.y) {
        for(x=a.x;x>=b.x;x--) if(forbidmap[x][y])break;
        if(x>=b.x)status=false;
        return status;
    }
}
int PATHPLANNING::path_planning(point depart, point dest, int boolean) //return total steps 
{
    if(boolean==true)proute = route;
    int x=depart.x, y=depart.y;

    point a,b;
    a.x = depart.x;
    a.y = dest.y;
    b.x = dest.x;
    b.y = depart.y;

    if(depart.x==dest.x && depart.y<dest.y) {
        if(validate_linepath(depart,dest)) {
            rotate(direction,FORWARD);
            for(y=depart.y;y<dest.y-1;y++) *proute++ = GOSTRAIGHT;
            direction = FORWARD;
            position = dest;
            return (proute-route);
        }
        else {
            if(direction == RIGHTWARD || direction == BACKWARD) {
                rotate(direction,RIGHTWARD);
                *proute++ = TURNLEFT;
                for(y=depart.y;y<dest.y-1;y++) *proute++ = GOSTRAIGHT;
                *proute++ = TURNLEFT;
                direction = LEFTWARD;
            }
            else if(direction == LEFTWARD || direction == FORWARD) {
                rotate(direction,LEFTWARD);
                *proute++ = TURNRIGHT;
                for(y=depart.y;y<dest.y-1;y++) *proute++ = GOSTRAIGHT;
                *proute++ = TURNRIGHT;
                direction = RIGHTWARD;
            }
            position = dest;
            return (proute-route);
        }
    }
    else if(depart.x==dest.x && depart.y>dest.y) {
        if(validate_linepath(depart,dest)) {
            rotate(direction,BACKWARD);
            for(y=depart.y;y>dest.y+1;y--) *proute++ = GOSTRAIGHT;
            direction = BACKWARD;
            position = dest;
            return (proute-route);
        }
        else {
            if(direction == RIGHTWARD || direction == BACKWARD) {
                rotate(direction,RIGHTWARD);
                *proute++ = TURNRIGHT;
                for(y=depart.y;y>dest.y+1;y--) *proute++ = GOSTRAIGHT;
                *proute++ = TURNRIGHT;
                direction = LEFTWARD;
            }
            else if(direction == LEFTWARD || direction == FORWARD) {
                rotate(direction,LEFTWARD);
                *proute++ = TURNLEFT;
                for(y=depart.y;y>dest.y+1;y--) *proute++ = GOSTRAIGHT;
                *proute++ = TURNLEFT;
                direction = RIGHTWARD;
            }
            position = dest;
            return (proute-route);
        }
    }
    else if(depart.x<dest.x && depart.y==dest.y) {
        if(validate_linepath(depart,dest)) {
            rotate(direction,RIGHTWARD);
            for(x=depart.x;x<dest.x-1;x++) *proute++ = GOSTRAIGHT;
            direction = RIGHTWARD;
            position = dest;
            return (proute-route);
        }
        else {
            if(direction == RIGHTWARD || direction == BACKWARD) {
                rotate(direction,BACKWARD);
                *proute++ = TURNLEFT;
                for(x=depart.x;x<dest.x-1;x++) *proute++ = GOSTRAIGHT;
                *proute++ = TURNLEFT;
                direction = FORWARD;
            }
            else if(direction == LEFTWARD || direction == FORWARD) {
                rotate(direction,FORWARD);
                *proute++ = TURNRIGHT;
                for(x=depart.x;x<dest.x-1;x++) *proute++ = GOSTRAIGHT;
                *proute++ = TURNRIGHT;
                direction = BACKWARD;
            }
            position = dest;
            return (proute-route);
        }
    }
    else if(depart.x>dest.x && depart.y==dest.y) {
        if(validate_linepath(depart,dest)) {
            rotate(direction,LEFTWARD);
            for(x=depart.x;x>dest.x+1;x--) *proute++ = GOSTRAIGHT;
            direction = LEFTWARD;
            position = dest;
            return (proute-route);
        }
        else {
            if(direction == RIGHTWARD || direction == BACKWARD) {
                rotate(direction,BACKWARD);
                *proute++ = TURNRIGHT;
                for(x=depart.x;x>dest.x+1;x--) *proute++ = GOSTRAIGHT;
                *proute++ = TURNRIGHT;
                direction = FORWARD;
            }
            else if(direction == LEFTWARD || direction == FORWARD) {
                rotate(direction,FORWARD);
                *proute++ = TURNLEFT;
                for(x=depart.x;x>dest.x+1;x--) *proute++ = GOSTRAIGHT;
                *proute++ = TURNLEFT;
                direction = BACKWARD;
            }
            position = dest;
            return (proute-route);
        }
    }
    else {
        if(validate_linepath(depart,a)&&validate_linepath(a,dest)) {
            path_planning(depart,a,false);
            path_planning(a,dest,false);
            return (proute-route);
        }
        else if(validate_linepath(depart,b)&&validate_linepath(b,dest)) {
            path_planning(depart,b,false);
            path_planning(b,dest,false);
            return (proute-route);
        }
        else {
            if(depart.x<dest.x) a.x = depart.x + 1;
            else a.x = depart.x - 1;
            a.y = depart.y;
            b.x = a.x;
            b.y = dest.y;
            path_planning(depart,a,false);
            path_planning(a,b,false);
            path_planning(b,dest,false);
            return (proute-route);
        }
    }
}
void PATHPLANNING::clear()
{
    proute=route;
    direction=FORWARD;
}
int PATHPLANNING::unload()
{
    *(proute++)=UNLOAD;
    direction = LEFTWARD;
    return (proute-route);
}