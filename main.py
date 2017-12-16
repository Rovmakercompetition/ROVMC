#!/usr/bin/env python

import socket
import sys
import time
import pigpio
import os

pin = [4,5,13]       #pins being used for servo signal to ESCs
dir = [128,128,128]
gain = 0.5          #gain to lower power (max 1)
motor = [1500, 1500, 1500]

#object initialization
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
pi = pigpio.pi()

if not pi.connected:
    exit()

#socket binding - should be rasp pi's IP and 8008 port
#Change IP adress to the IP of your RASPBERRY PI
sock.bind(("169.254.165.78",8008))

#ESC initialization
pi.set_servo_pulsewidth(pin[0], 1500)
pi.set_servo_pulsewidth(pin[1], 1500)
pi.set_servo_pulsewidth(pin[2], 1500)
time.sleep(1.5)

print("Press Ctrl+C to interrupt")

#functions
def setMotor(mot, value):
    pi.set_servo_pulsewidth(pin[mot], value)

def make8bit(number):
    while number > 255:
        number-=256
    return number

def remap(number):
    if number == 128:
        return 0
    else:
        return (((number - 128) * 500)/128)*gain
    
#main loop
while True:
    try:
        #reset data
        data = "\x80\x80\x80\x00\x80"
        #receive data - timeout of quarter of second
        try:
            sock.settimeout(0.25)
            data, addr = sock.recvfrom(1024)
        except socket.timeout:
            print("disconnected")
        #checksum calculation
        num = ord(data[0])+ord(data[1])+ord(data[2])+ord(data[3])
        num8 = make8bit(num)
        #reset of the values
        motor[0]=1500
        motor[1]=1500
        motor[2]=1500
        dir[0]=128  #f/b
        dir[1]=128  #l/r
        dir[2]=128  #u/d
        if ord(data[4]) == num8:   #check sum
            if ord(data[3]) == 1:  #disarm check
                #direction assignment
                dir[0]=ord(data[0])
                dir[1]=ord(data[1])
                dir[2]=ord(data[2])
        #motor power distribution (to be done!!!)
        motor[0] += (remap(dir[0]) - remap(dir[1]))/2 #left motor
        motor[1] += (-remap(dir[0]) - remap(dir[1]))/2 #right motor
        motor[2] += remap(dir[2])                     #vertical
        setMotor(0,motor[0])
        setMotor(1,motor[1])
        setMotor(2,motor[2])
    except KeyboardInterrupt:
        #motor reset
        setMotor(0,1500)
        setMotor(1,1500)
        setMotor(2,1500)
        break

#deinitialize
print("\nDeinitializing the peripherals")

pi.stop()
