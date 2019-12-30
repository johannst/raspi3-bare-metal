//
// raspi3-bare-metal - Toy project for personal education
// Copright (C) 2019 Johannes Stoelp <johannes.stoelp@gmail.com>
//

#include "gpio.h"
#include "peripherals.h"

// GPFSEL - GPIO Funcionn Select
//    GPFSEL0 [2:0] - Function Select GPIO0
//    GPFSEL0 [5:3] - Function Select GPIO1
//    ...
//    GPFSEL0 [29:27] - Function Select GPIO9
//    GPFSEL0 [31:30] - Reserved
//    GPFSEL1 [2:0] - Function Select GPIO10
//    ...
void gpio_set_function(int port, GpioFunc func) {
   int port_sel = port/10;
   int port_index = 3*(port%10);

   uint32_t reg = GPIO.GPFSEL[port_sel];
   reg &= ~(7 << port_index);
   reg |= (func << port_index);
   GPIO.GPFSEL[port_sel];
}

// Protocol to change GPPUD:
//   1. Wirte PullUp/Down config to GPPUD.
//   2. Wait 150 cycles -> set-up time for the control signal
//   3. Enable GPIO clock(s) GPPUDCLK for the pads that should be affected by
//      the config in GPPUD.
//   4. Wait 150 cycles -> hold time for the control signal
//   5. Reset GPPUD.
//   6. Disable GPIO clock(s).
void gpio_set_pull_resistor(int port, GpioPullResistor pud) {
   int port_sel = port/32;
   int port_index = (port%32);

   GPIO.GPPUD = pud;
   for (int i=0; i<150; ++i) { __asm__ ("nop"); }
   GPIO.GPPUDCLK[port_sel] = (1 << port_index);
   for (int i=0; i<150; ++i) { __asm__ ("nop"); }
   GPIO.GPPUD = 0x00;
   GPIO.GPPUDCLK[port_sel] = 0;
}

// vim:et:ts=4

