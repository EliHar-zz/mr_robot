# SPI ATMEGA328 (Slave) and BBB (Master)

### Example steps
- Load the program slave.c into the ATMEGA328
- Disable the HDMI because it conflicts with SPI. To do that, copy the content of uEnv.txt into the BBB /boot/uEnv.txt (In fact, only the line containing the HDMI is being read)
- Make sure you have the SPIDEV0 file under `/lib/firmware/` by typing: `ls /lib/firmware/*SPI*`. If it's not there, downloaded it from https://github.com/jadonk/cape-firmware/blob/master/arch/arm/boot/dts/BB-SPIDEV0-00A0.dts and compile it with `dtc`, (e.g. `dtc -O dtb -o BB-SPI1-01-00A0.dtbo -b 0 -@ BB-SPI1-01-00A0.dts`) then copy the new dtbo file created into `/lib/firmware/`
- Load SPIDEV0 into the slots `echo BB-SPIDEV0 > /sys/devices/bone_capemgr.9/slots` (If you already loaded other SPI dtbo before, restart your BBB and load SPIDEV0 only or it won't work)
- Open the Serial monitor for the ATMEGA328 (Slave). You can use the tool under https://github.com/EliHar/mr_robot/blob/master/tools/tty/monitor or the Arduino IDE
- Compile the spi_test.c file `gcc -o spi_test spi_test.c`. This file is a modified version of the one provided at https://github.com/raspberrypi/linux/blob/rpi-4.4.y/Documentation/spi/spidev_test.c also a copy of the original file is added in this directory.
- Run the compiled spi_test program and you should see the ascii values for the numbers 1,2 and 3 appear on the Serial monitor.
- After running the spi_test at least one time, you can use the tool under https://github.com/EliHar/mr_robot/blob/master/tools/tty/write to send messages from the BBB to the ATMEGA328 (Limitation to be investigated)

### To do
- Extend the code for `spi_test.c` to allow writting custom messages by passing `--message` flag
- Write a script to load BB-SPIDEV0 into the slots at startup
