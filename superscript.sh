#!/bin/bash

sudo pigpiod
python main.py & \
#Change host to the IP of your laptop
raspivid -t 0 -vf -hf -n -w 1280 -h 720 -fps 60 -b 25000000 -o - | \
gst-launch-1.0 -v fdsrc ! h264parse ! rtph264pay config-interval=10 \
pt=96 ! udpsink host=<IP of your computer> port=5000
