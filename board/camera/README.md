# Camera on BBB

### Table of Contents

  * [Software required](#software-required)
  * [Camera configuration](#camera-configuration)
  * [Troubleshoot](#troubleshoot)
  * [Reference](#reference)
  
### Software required

  * v4l-utils
    * `apt-get install v4l-utils v4l-utils`
  * BoneCV
    * `git clone https://github.com/derekmolloy/boneCV`
    * Build the project: `cd boneCV && ./build`

### Camera configuration

*Make sure your USB camera is plugged to the beaglebone before proceeding*

  * To check what are the format supported by the camera:  
    `v4l2-ctl --list-formats`
    
  * To check which format is activated:  
    `v4l2-ctl --all`
    
  * To change video format to index 1 (just an example):  
    `v4l2-ctl --set-fmt-video=pixelformat=1`
  
### Troubleshoot

  * If you get **select timeout** run the following in your BBB terminal:
```
# Reference: https://www.raspberrypi.org/forums/viewtopic.php?t=35689&p=300710
rmmod uvcvideo
modprobe uvcvideo nodrop=1 timeout=5000 quirks=0x80
```

  * If you get **pipe:: Invalid data found when processing input**, then make sure you have the avconv flags set in the correct order first, then try different arguments. Docs: https://libav.org/avconv.html
  

### Streaming

#### Streaming from BBB to my PC on port 1234 over UDP

##### BBB

`./capture -o -c0 | avconv -f mjpeg -i pipe:0 -f mpegts udp://192.168.1.187:1234`

##### PC

`vlc udp://@:1234`

### Reference

  Derek Molloy:

  * Website: http://derekmolloy.ie/beaglebone/beaglebone-black-streaming-video-tutorial/#Introduction
  * Youtube: https://www.youtube.com/watch?v=8QouvYMfmQo and https://www.youtube.com/watch?v=-6DBR8PSejw
