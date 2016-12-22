#!/bin/bash

while true
do
    say `curl -s http://192.168.0.150/mr_robot/info.txt | grep Direction | sed 's/Direction: //g'` 
    sleep 2
done
