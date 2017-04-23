On kernel 3.x:  
```
gcc -o write write.c
./write 255,255#
```

On kernel 4.x:
```
gcc -o write write.c
./write 255,255#
```

Both write.c are a modified version of https://github.com/raspberrypi/linux/blob/rpi-4.4.y/Documentation/spi/spidev_test.c

