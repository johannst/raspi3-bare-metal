//
// raspi3-bare-metal - Toy project for personal education
// Copright (C) 2019 Johannes Stoelp <johannes.stoelp@gmail.com>
//

#include "peripherals.h"
#include "gpio.h"

void mu_initialize() {
    // enable MiniUart (UART1), no access to registers if not enabled
    AUX.AUX_ENABLES |= 1;

    // disable transmitter & receiver
    MINI_UART.AUX_MU_CNTL = 0;

    // disable interrupts
    MINI_UART.AUX_MU_IER = 0;

    // [2]: write 1 -> clear transmit FIFO
    // [1]: write 1 -> clear receive FIFO
    MINI_UART.AUX_MU_IIR = 0x06;

    // data size: 8bit, DLAB=0
    MINI_UART.AUX_MU_LCR = 3;

    // baudrate = f_sys_clk / (8*(AUX_MU_BAUD+1))
    // default f_sys_clk = 250MHz
    // set 115200 baud
    MINI_UART.AUX_MU_BAUD = 270;

    /* map UART1 to GPIO pins */
    gpio_set_function(14, IO_FUNC_ALT5); // TXD1
    gpio_set_function(15, IO_FUNC_ALT5); // RXD1

    // disable pull-up/pull-down
    gpio_set_pull_resistor(14, IO_PUD_OFF);
    gpio_set_pull_resistor(15, IO_PUD_OFF);

    // [1]: transmitter enable
    // [0]: receiver enable
    MINI_UART.AUX_MU_CNTL = 3;
}

void mu_putc(char c) {
#define TX_IDLE (1 << 6)
    while (!(MINI_UART.AUX_MU_LSR & TX_IDLE)) {
        __asm__ ("nop");
    }
    MINI_UART.AUX_MU_IO = c;
}

char mu_getc() {
#define RX_FIFO_HAS_SYMBOL 0x1
    while (!(MINI_UART.AUX_MU_STAT & RX_FIFO_HAS_SYMBOL)) {
        __asm__ ("nop");
    }
    char r = (char)(MINI_UART.AUX_MU_IO);
    return r=='\r' ? '\n' : r;
}

void mu_puts(const char *s) {
    while (*s) {
        if (*s == '\n') {
            mu_putc('\r');
        }
        mu_putc(*s++);
    }
}

int mu_receiver_overrun_occured(int clear_on_read) {
    if (clear_on_read == 0) {
        return (MINI_UART.AUX_MU_STAT & (1 << 4)) >> 4;
    }
    return (MINI_UART.AUX_MU_LSR & (1 << 1)) >> 1;
}

// vim:et:ts=4

