from test import check
from ser import command,capt
import numpy as np
import time
import cv2
curx=0
cury=0
facing=0
command(b'1')
def cap():
    frame=capt()
    uppic=frame[:240,200:440,:]
    downpic=frame[240:,200:440,:]
#    cv2.imshow('frame',frame)
#    cv2.waitKey(0)
    cv2.imwrite('up.jpg',uppic)
    cv2.imwrite('down.jpg',downpic)
    r1,b1=check(uppic)
    r2,b2=check(downpic)
#     return 1,1
    return (r1 or b1),(r2 or b2)
def turn(f):
    global facing
    if(f==facing):
        return
    if(facing==(f+1)%4):
        command(b'3')
        facing=facing-1
        if(facing<0):
            facing=3
    else:
        while(facing!=f):
            command(b'2')
            facing=(facing+1)%4
def go(x,y):
    global curx,cury
    print(x,y)
    if(x>curx):
        turn(0)
        while(x>curx):
            command(b'1')
            curx=curx+1
    if(y<cury):
        turn(1)
        while(y<cury):
            command(b'1')
            cury=cury-1
    if(x<curx):
        turn(2)
        while(x<curx):
            command(b'1')
            curx=curx-1
    if(y>cury):
        turn(3)
        while(y>cury):
            command(b'1')
            cury=cury+1
def del_up(x,y,f):
    go(x,y)
    turn(f)
    command(b'u')
    if(f):
        go(x,y+1)
    else:
        go(x-1,y)
    command(b'b')
    go(1,0)
    turn(2)
    command(b'h')
    command(b'd')
    command(b'b')
    command(b'H')
#    go(x,y)
#    turn(f)
#    command(b'l')
#    command(b'b')
#    go(1,0)
#    turn(2)
#    command(b'd')
#    go(x,y)
#    turn(f)
#    command(b'r')
#    command(b'b')
#    go(1,0)
#    turn(2)
#    command(b'd')
    
def del_down(x,y,f):
    go(x,y)
    turn(f)
    command(b'D')
    if(f):
        go(x,y+1)
    else:
        go(x-1,y)
    command(b'b')
    go(1,0)
    turn(2)
    command(b'h')
    command(b'd')
    command(b'b')
    command(b'H')
#    go(x,y)
#    turn(f)
#    command(b'L')
#    command(b'b')
#    go(1,0)
#    turn(2)
#    command(b'd')
#    go(x,y)
#    turn(f)
#    command(b'R')
#    command(b'b')
#    go(1,0)
#    turn(2)
#    command(b'd')
l=5
for i in range(1,l+1):
    go(i,-1)
    turn(0)
    time.sleep(0.2)
    up,down=cap()
    if(up):
        del_up(i,-2,1)
    if(down):
        del_down(i,-2,1)
for i in range(-1,5):
    go(8,i)
    turn(3)
    up,down=cap()
    if(up):
        del_up(9,i,0)
    if(down):
        del_down(9,i,0)