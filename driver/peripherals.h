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

#ifdef RPI3
# define PERIPH_BASE 0x3F000000
#else
# error Peripherals not implemented!
#endif

#define AUX       (*(volatile struct AuxRegs*)     (PERIPH_BASE+0x215000))
#define MINI_UART (*(volatile struct MiniUartRegs*)(PERIPH_BASE+0x215040))
#define GPIO      (*(volatile struct GpioRegs*)    (PERIPH_BASE+0x200000))

// vim:et:ts=4

