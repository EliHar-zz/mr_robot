On kernel 3.x:  
```
gcc -o write write.c
./write 255,255#
```

On kernel 4.x:
```
gcc -o write spidev_test.c
./write -s 10000 -p "20,10#" -D /dev/spidev1.0 --bpw 8 -v
```

write.c is a modified version of https://github.com/raspberrypi/linux/blob/rpi-4.4.y/Documentation/spi/spidev_test.c

spidev_test.c is the same as https://github.com/raspberrypi/linux/blob/rpi-4.4.y/Documentation/spi/spidev_test.c
