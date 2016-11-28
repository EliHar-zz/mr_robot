#!/bin/bash

# This program blinks pin 9-41 (GPIO_20)

DIR=/sys/class/gpio

# If it isn't already, get gpio20 to appear in DIR 
if [ ! -e ${DIR}/gpio20 ]
then
    echo 20 > ${DIR}/export
fi

# Set pin as output
echo high > ${DIR}/gpio20/direction

# Run infinite loop to blink led
while :
do
    # turn led on and wait for half a second
    echo 1 > ${DIR}/gpio20/value
    sleep .5

    # turn led off and wait for half a second
    echo 0 > ${DIR}/gpio20/value
    sleep .5
done
