// GPIO driver
// Adapted from http://elinux.org/RPi_Low-level_peripherals by <weldale@gmail.com>
// 2012

#ifndef __GPIO_DRIVER_H
#define __GPIO_DRIVER_H

#define BCM2708_PERI_BASE        0x20000000
#define GPIO_BASE                (BCM2708_PERI_BASE + 0x200000) /* GPIO controller */

#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)

#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))

#define USE_GPIO(g) INP_GPIO(g); OUT_GPIO(g)

#define GPIO_SET *(gpio+7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR *(gpio+10) // clears bits which are 1 ignores bits which are 0

#define GPIO_SETX(g) GPIO_SET = 1 << (g)
#define GPIO_CLRX(g) GPIO_CLR = 1 << (g)

void gpio_setup();

extern volatile unsigned *gpio;

#endif

