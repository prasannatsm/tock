#include <firestorm.h>

#define LED_1 19

/* FIXME: These delay functions are Cortex-M0 specific (and calibrated for a
 * 16MHz CPU clock), therefore should be moved to platform specific location.
 * */

/* Delay for for the given microseconds (approximately).
 *
 * For a 16 MHz CPU, 1us == 16 instructions (assuming each instruction takes
 * one cycle). */
static void delay_us(int duration)
{
	// The inner loop instructions are: 14 NOPs + 1 SUBS/ADDS + 1 CMP
	while (duration-- != 0) {
		__asm volatile (
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
		);
	}
}

/* Delay for for the given milliseconds (approximately).
 *
 * Note that this is not precise as there are 2 extra instructions on the inner
 * loop. Therefore, there is 1us added every 8 iterations. */
static void delay_ms(int duration)
{
	while (duration-- != 0) {
		delay_us(1000);
	}
}

char rbuf[200];
char wbuf[200];

void main(void)
{
        int i;
	gpio_enable(LED_0);
	gpio_enable(LED_1);

	for (i = 0; i < 200; i++) {
                rbuf[i] = 0;
		wbuf[i] = i;
	}
//	spi_read_buf(rbuf, 200);
    while (1) { 
	gpio_set(LED_1);
	gpio_clear(LED_0);

	spi_write(0xaa);

	gpio_clear(LED_1);
	gpio_set(LED_0);
	spi_repeat_write(wbuf, 7, rbuf);
	wait();
	delay_ms(25);
    }
}
