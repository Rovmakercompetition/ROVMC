#!/bin/bash

sudo pigpiod
python main.py & \
#Change host to the IP of your laptop
raspivid -t 0 -vf -hf -n -w 640 -h 360 -fps 30 -b 25000000 -o - | \
gst-launch-1.0 -v fdsrc ! h264parse ! rtph264pay config-interval=10 \
pt=96 ! udpsink host=169.254.0.52 port=5000
