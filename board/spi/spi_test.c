/*
 * Original file: https://github.com/raspberrypi/linux/blob/rpi-4.4.y/Documentation/spi/spidev_test.c
 *
 * New changes in this version:
 * - Changed default device to /dev/spidev1.0
 * - Changed mode default value to 0
 * - Changed speed default value to 0
 * - Changed `if` condition for executing parbort("can't send spi message") 
 * - Read tx values from the user
 *
 * SPI testing utility (using spidev driver)
 *
 * Copyright (c) 2007  MontaVista Software, Inc.
 * Copyright (c) 2007  Anton Vorontsov 
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 * Cross-compile with cross-gcc -I/path/to/cross-kernel/include
 *
 */

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <string.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

static void pabort(const char *s)
{
    perror(s);
    abort();
}

// Set default to spidev1.0
static const char *device = "/dev/spidev1.0";
static uint8_t mode = 0;
static uint8_t bits = 0;
static uint16_t delay = 0;

// This speed value works
static uint32_t speed = 0;

char *input_tx;

static void hex_dump(const void *src, size_t length, size_t line_size, char *prefix)
{
    int i = 0;
    const unsigned char *address = src;
    const unsigned char *line = address;
    unsigned char c;

    printf("%s | ", prefix);
    while (length-- > 0) {
        printf("%02X ", *address++);
        if (!(++i % line_size) || (length == 0 && i % line_size)) {
            if (length == 0) {
                while (i++ % line_size)
                    printf("__ ");
            }
            printf(" | ");  /* right close */
            while (line < address) {
                c = *line++;
                printf("%c", (c < 33 || c == 255) ? 0x2E : c);
            }
            printf("\n");
            if (length > 0)
                printf("%s | ", prefix);
        }
    }
}

/*
 *  Unescape - process hexadecimal escape character
 *      converts shell input "\x23" -> 0x23
 */
static int unescape(char *_dst, char *_src, size_t len)
{
    int ret = 0;
    char *src = _src;
    char *dst = _dst;
    unsigned int ch;

    while (*src) {
        if (*src == '\\' && *(src+1) == 'x') {
            sscanf(src + 2, "%2x", &ch);
            src += 4;
            *dst++ = (unsigned char)ch;
        } else {
            *dst++ = *src++;
        }
        ret++;
    }
    return ret;
}

static void transfer(int fd, uint8_t const *tx, uint8_t const *rx, size_t len)
{
    int ret;

    struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long)tx,
        .rx_buf = (unsigned long)rx,
        .len = len,
        .delay_usecs = delay,
        .speed_hz = speed,
        .bits_per_word = bits,
    };

    ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
    if (ret < 1)
        pabort("can't send spi message");

    hex_dump(rx, len, 32, "RX");
}

void print_usage(const char *prog)
{
    printf("Usage: %s [-DsbdlHOLpC3]\n", prog);
    puts("  -D --device   device to use (default /dev/spidev1.0)\n"
            "  -s --speed    max speed (Hz)\n"
            "  -d --delay    delay (usec)\n"
            "  -b --bpw      bits per word \n"
            "  -l --loop     loopback\n"
            "  -H --cpha     clock phase\n"
            "  -O --cpol     clock polarity\n"
            "  -p            Send data (e.g. \"1234\\xde\\xad\")\n"
            "  -L --lsb      least significant bit first\n"
            "  -C --cs-high  chip select active high\n"
            "  -3 --3wire    SI/SO signals shared\n");
    exit(1);
}

void parse_opts(int argc, char *argv[])
{
    while (1) {
        static const struct option lopts[] = {
            { "device",  1, 0, 'D' },
            { "speed",   1, 0, 's' },
            { "delay",   1, 0, 'd' },
            { "bpw",     1, 0, 'b' },
            { "loop",    0, 0, 'l' },
            { "cpha",    0, 0, 'H' },
            { "cpol",    0, 0, 'O' },
            { "lsb",     0, 0, 'L' },
            { "cs-high", 0, 0, 'C' },
            { "3wire",   0, 0, '3' },
            { NULL, 0, 0, 0 },
        };
        int c;

        c = getopt_long(argc, argv, "D:s:d:b:lHOLC3p:", lopts, NULL);

        if (c == -1)
            break;

        switch (c) {
            case 'D':
                device = optarg;
                break;
            case 's':
                speed = atoi(optarg);
                break;
            case 'd':
                delay = atoi(optarg);
                break;
            case 'b':
                bits = atoi(optarg);
                break;
            case 'l':
                mode |= SPI_LOOP;
                break;
            case 'H':
                mode |= SPI_CPHA;
                break;
            case 'O':
                mode |= SPI_CPOL;
                break;
            case 'L':
                mode |= SPI_LSB_FIRST;
                break;
            case 'C':
                mode |= SPI_CS_HIGH;
                break;
            case '3':
                mode |= SPI_3WIRE;
                break;
            case 'p':
                input_tx = optarg;
                break;
            default:
                print_usage(argv[0]);
                break;
        }
    }
}

int main(int argc, char *argv[])
{
    int ret = 0;
    int fd;
    uint8_t *tx;
    uint8_t *rx;
    int size;

    parse_opts(argc, argv);

    fd = open(device, O_RDWR);
    if (fd < 0)
        pabort("can't open device");

    /*
     * spi mode
     */
    ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
    if (ret == -1)
        pabort("can't set spi mode");

    ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
    if (ret == -1)
        pabort("can't get spi mode");

    /*
     * bits per word
     */
    ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    if (ret == -1)
        pabort("can't set bits per word");

    ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
    if (ret == -1)
        pabort("can't get bits per word");

    /*
     * max speed hz
     */
    ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    if (ret == -1)
        pabort("can't set max speed hz");

    ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
    if (ret == -1)
        pabort("can't get max speed hz");

    printf("spi mode: %d\n", mode);
    printf("bits per word: %d\n", bits);
    printf("max speed: %d Hz (%d KHz)\n", speed, speed/1000);

    if (input_tx) {
        size = strlen(input_tx+1);
        tx = malloc(size);
        rx = malloc(size);
        puts("Calculating size ...");
        size = unescape((char *)tx, input_tx, size);
        printf("size: %d\n", size);
        puts("Starting transfer ...");
        transfer(fd, tx, rx, size);
        free(rx);
        free(tx);
    } else {
        print_usage(argv[0]);
    }

    close(fd);

    return ret;
}
