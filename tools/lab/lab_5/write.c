/*
 * This program uses the logic of the spi test file: https://github.com/EliHar/mr_robot/blob/master/board/spi/spi_test.c
 * Use this program to send input data from the BBB to the ATMega using SPI
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
#include <math.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

static void pabort(const char *s)
{
	perror(s);
	abort();
}

static const char *device = "/dev/spidev1.0";
static uint8_t mode = 0;
static uint8_t bits = 0;
static uint16_t delay = 0;
char *input_tx;

// This speed value works
static uint32_t speed = 0;

/*
 *  Unescape - process hexadecimal escape character
 *      converts shell input "\x23" -> 0x23
 */
static int parse(char *_dst, char *_src, size_t len)
{
	int ret = 0;
	char *src = _src;
	char *dst = _dst;
	unsigned int ch;

	while (*src) {
		*dst++ = *src++;
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
		.speed_hz = 0,
		.bits_per_word = 0,
	};

	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 1)
		pabort("can't send spi message");
}

void print_usage(char *str) {
	printf("Program usage:\n    %s <message>\n", str);
}

int main(int argc, char *argv[]) {
	int ret = 0;
	int fd;
	uint8_t *tx;
	uint8_t *rx;
	int size;

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

	if(argc != 2) {
		printf("No message provided.");
		print_usage(argv[0]);
		return 1;
	}

	input_tx = argv[1];
	puts("Calculating size before parsing ...");
	size = strlen(input_tx);

	printf("size: %d\n", size);

	// Allocate memory
	tx = malloc(size);
	rx = malloc(size);

	puts("Calculating size after parsing ...");
	size = parse((char *)tx, input_tx, size);

	printf("size: %d\n", size);

	puts("Starting transfer ...");
	transfer(fd, tx, rx, size);
	free(rx);
	free(tx);

	close(fd);

	return ret;
}

