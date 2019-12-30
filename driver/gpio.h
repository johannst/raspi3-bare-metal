//
// raspi3-bare-metal - Toy project for personal education
// Copright (C) 2019 Johannes Stoelp <johannes.stoelp@gmail.com>
//

#pragma once

typedef enum {
   IO_FUNC_IN   = 0x00,
   IO_FUNC_OUT  = 0x01,
   IO_FUNC_ALT0 = 0x04,
   IO_FUNC_ALT1 = 0x05,
   IO_FUNC_ALT2 = 0x06,
   IO_FUNC_ALT3 = 0x07,
   IO_FUNC_ALT4 = 0x03,
   IO_FUNC_ALT5 = 0x02,
} GpioFunc;

typedef enum {
   IO_PUD_OFF  = 0x00,
   IO_PUD_DOWN = 0x01,
   IO_PUD_UP   = 0x02,
} GpioPullResistor;

void gpio_set_function(int port, GpioFunc func);
void gpio_set_pull_resistor(int port, GpioPullResistor pud);

// vim:et:ts=4

