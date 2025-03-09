#ifndef PATHPLANNING_H
#define PATHPLANNING_H
#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

#include "config.h"


class PATHPLANNING {
    public:
        PATHPLANNING();
        int path_planning(point depart, point dest, int boolean); //"boolean == true" means writing route[] from start, otherwise adding from the tail  
        void clear();
        int unload();
        int route[500];
        int direction = FORWARD; //the current direction state of the car
        point position;
        const point dest1 = {6,3}, dest2 = {6,5}; //destination of the rings
        const point depart_left = {1,0}, depart_right = {5,0}; //can be changed
        
    private:
        const int forbidmap[7][9] = {{0,0,1,0,1,0,1,0,0},{0,0,0,0,0,0,0,0,0},{0,0,0,0,1,0,0,0,0},{0,0,0,0,0,0,0,0,0},{0,0,0,0,1,0,0,0,0},{0,0,0,0,0,0,0,0,0},{0,0,1,0,1,0,1,0,0}}; //the location where the car cannot arrive
        
        
        
        int *proute=route;

        void rotate(int initial_direction,int final_direction);
        int validate_linepath(point a, point b);
        
};
#endif