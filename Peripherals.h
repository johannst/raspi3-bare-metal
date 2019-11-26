//
// raspi3-bare-metal - Toy project for personal education
// Copright (C) 2019 Johannes Stoelp <johannes.stoelp@gmail.com>
//

#pragma once
#include <stdint.h>

struct AuxRegs {
   uint32_t AUX_IRQ;
   uint32_t AUX_ENABLES;
};

struct MiniUartRegs {
   uint32_t AUX_MU_IO;      // I/O Data
   uint32_t AUX_MU_IER;     // Interrupt Enable
   uint32_t AUX_MU_IIR;     // Interrupt Identify
   uint32_t AUX_MU_LCR;     // Line Control
   uint32_t AUX_MU_MCR;     // Modem Control
   uint32_t AUX_MU_LSR;     // Line Status
   uint32_t AUX_MU_MSR;     // Modem Status
   uint32_t AUX_MU_SCRATCH; // Scratch
   uint32_t AUX_MU_CNTL;    // Extra Control
   uint32_t AUX_MU_STAT;    // Extra Status
   uint32_t AUX_MU_BAUD;    // Baudrate
};

struct GpioRegs {
   uint32_t GPFSEL[6];   // Function Select 0-5 [R/W]
   uint32_t reserve0;
   uint32_t GPSET[2];    // Pin Output Set 0-1 [W]
   uint32_t reserve1;
   uint32_t GPCLR[2];    // Pin Output Clear 0-1 [W]
   uint32_t reserve2;
   uint32_t GPLEV[2];    // Pin Level 0-1 [R]
   uint32_t reserve3;
   uint32_t GPEDS[2];    // Pin Event Detect Status 0-1 [R/W]
   uint32_t reserve4;
   uint32_t GPREN[2];    // Pin Rising Edge Detect Enable 0-1 [R/W]
   uint32_t reserve5;
   uint32_t GPFEN[2];    // Pin Falling Edge Detect Enable 0-1 [R/W]
   uint32_t reserve6;
   uint32_t GPHEN[2];    // Pin High Detect Enable 0-1 [R/W]
   uint32_t reserve7;
   uint32_t GPLEN[2];    // Pin Low Detect Enable 0-1 [R/W]
   uint32_t reserve8;
   uint32_t GPAREN[2];   // Pin Async. Rising Edge Detect 0-1 [R/W]
   uint32_t reserve9;
   uint32_t GPAFEN[2];   // Pin Async. Falling Edge Detect 0-1 [R/W]
   uint32_t reserve10;
   uint32_t GPPUD;       // Pin Pull-up/down Enable [R/W]
   uint32_t GPPUDCLK[2]; // Pin Pull-up/down Enable Clock 0-1 [R/W]
};

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

#ifdef RPI3
# define PERIPH_BASE 0x3F000000
#else
# error Peripherals not implemented!
#endif

#define AUX       (*(volatile struct AuxRegs*)     (PERIPH_BASE+0x215000))
#define MINI_UART (*(volatile struct MiniUartRegs*)(PERIPH_BASE+0x215040))
#define GPIO      (*(volatile struct GpioRegs*)    (PERIPH_BASE+0x200000))

// GPFSEL - GPIO Funcionn Select
//    GPFSEL0 [2:0] - Function Select GPIO0
//    GPFSEL0 [5:3] - Function Select GPIO1
//    ...
//    GPFSEL0 [29:27] - Function Select GPIO9
//    GPFSEL0 [31:30] - Reserved
//    GPFSEL1 [2:0] - Function Select GPIO10
//    ...
void setGpioFunction(int port, GpioFunc func) {
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
void setGpioPullResistor(int port, GpioPullResistor pud) {
   int port_sel = port/32;
   int port_index = (port%32);

   GPIO.GPPUD = pud;
   for (int i=0; i<150; ++i) { asm volatile("nop"); }
   GPIO.GPPUDCLK[port_sel] = (1 << port_index);
   for (int i=0; i<150; ++i) { asm volatile("nop"); }
   GPIO.GPPUD = 0x00;
   GPIO.GPPUDCLK[port_sel] = 0;
}

// vim:et:ts=4

