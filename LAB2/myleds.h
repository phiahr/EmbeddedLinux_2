/* The declarations here have to be in a header file, because
 *  they need to be known both to the kernel module
 *  (in mydevice.c) and the process calling ioctl (test_driver.c) */

#include <linux/ioctl.h>

#define MYDEVICE_MAJOR 60
#define MYDEVICE_PATH "/dev/myleds"
#define MAX_BUFFER_SIZE 100

#define XPAR_LEDS_8BIT_BASEADDR 0x41200000
#define GPIO_DATA (*((unsigned int*)(XPAR_LEDS_8BIT_BASEADDR + 0x00)))
#define GPIO_TRI (*((unsigned int*)(XPAR_LEDS_8BIT_BASEADDR + 0x04)))


