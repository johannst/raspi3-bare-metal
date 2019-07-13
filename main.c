//
// raspi3-bare-metal - Toy project for personal education
// Copright (C) 2019 Johannes Stoelp <johannes.stoelp@gmail.com>
//

#define RPI3
#include "Peripherals.h"

void miniUartInitialize() {
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
    setGpioFunction(14, IO_FUNC_ALT5); // TXD1
    setGpioFunction(15, IO_FUNC_ALT5); // RXD1

    // disable pull-up/pull-down
    setGpioPullResistor(14, IO_PUD_OFF);
    setGpioPullResistor(15, IO_PUD_OFF);

    // [1]: transmitter enable
    // [0]: receiver enable
    MINI_UART.AUX_MU_CNTL = 3;
}

void miniUartPutc(char c) {
#define TX_IDLE 0x20
    while (!(MINI_UART.AUX_MU_LSR & TX_IDLE)) {
        asm volatile("nop");
    }
    MINI_UART.AUX_MU_IO = c;
}

char miniUartGetc() {
#define RX_READY 0x01
    while (!(MINI_UART.AUX_MU_LSR & RX_READY)) {
        asm volatile("nop");
    }
    char r = (char)(MINI_UART.AUX_MU_IO);
    return r=='\r' ? '\n' : r;
}

void miniUartPuts(const char *s) {
    while (*s) {
        if (*s == '\n') {
            miniUartPutc('\r');
        }
        miniUartPutc(*s++);
    }
}

int main() {
    miniUartInitialize();
    miniUartPuts("This works?\n");
    return 0;
}

// vim:et:ts=4

