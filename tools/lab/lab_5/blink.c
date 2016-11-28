#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
	FILE *fp;

	// If file doesn’t exist, create it
	if (access("/sys/class/gpio/gpio20", F_OK) == -1) {
		fp = fopen("/sys/class/gpio/export", "w+");
		fprintf(fp, "20");
		fclose(fp);
	}

	// Set pin to output
	fp = fopen("/sys/class/gpio/gpio20/direction", "w+");
	fprintf(fp, "high");
	fclose(fp);

	bool led_on = false;

	// Run infinite loop to blink LED
	for (;;) {
		fp = fopen("/sys/class/gpio/gpio20/value", "w+");
		if (led_on) {
			fprintf(fp, "0"); // turn off LED
		} else {
			fprintf(fp, "1"); // turn on LED
		}
		fclose(fp); // must be closed every iteration to get changes
		led_on = !led_on; // toggle boolean
		sleep(1); // wait 1 second
	}
}

