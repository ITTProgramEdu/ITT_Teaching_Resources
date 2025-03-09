import serial
import time
import cv2
serialPort = "/dev/ttyACM0"
baudRate=9600
ser=serial.Serial(serialPort,baudRate,timeout=0.5)
time.sleep(2)
def command(t):
    print(t)
    ser.write(t);
    while True:
        c=ser.read();
        #capture.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
        #capture.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)
        #read_code,frame=capture.read()
        #cv2.imshow('video',frame)
        if(c==b'd'):
            return

def capt():
    capture = cv2.VideoCapture(0)
    capture.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
    capture.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)
    for i in range(1,100):
        read_code, frame = capture.read()
    capture.release()
    return frame