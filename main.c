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

const char hexchars[] = "0123456789abcdef";
//#define hex2ascii(hex) hexchars[hex]
char hex2ascii(uint8_t nibble) {
    return hexchars[nibble];
}

void printx32(char* buf, uint32_t val) {
    const int bytes = 4;

    const int nibbles = 2*bytes;
    for (int i=0; i<nibbles; ++i) {
        buf[i] = hex2ascii(val >> 4*(nibbles-1-i) & 0x0f);
    }
    buf[2*bytes] = '\0';
}

void printx64(char* buf, uint64_t val) {
    const int bytes = 8;

    const int nibbles = 2*bytes;
    for (int i=0; i<nibbles; ++i) {
        buf[i] = hex2ascii(val >> 4*(nibbles-1-i) & 0x0f);
    }
    buf[2*bytes] = '\0';
}

// {{{ Atag

struct AtagHeader {
    uint32_t size;
    uint32_t tag;
};

struct AtagCore {
    uint32_t flags;
    uint32_t pagesize;
    uint32_t rootdev;
};

struct AtagMem {
    uint32_t size;
    uint32_t start;
};

struct AtagEntry {
    struct AtagHeader hdr;
    union {
        struct AtagCore core;
        struct AtagMem mem;
    };
};

enum eAtagType {
    ATAG_NONE = 0x00000000,
    ATAG_CORE = 0x54410001,
    ATAG_MEM  = 0x54410002,
};

void walkAtags(void* start_addr) {
    struct AtagEntry* atag = (struct AtagEntry*)start_addr;

    while (atag->hdr.tag != ATAG_NONE) {
        miniUartPuts("Found ATAG tag=");
        switch (atag->hdr.tag) {
            case ATAG_CORE:
                miniUartPuts("ATAG_CORE\n");
                break;
            case ATAG_MEM:
                miniUartPuts("ATAG_MEM\n");
                {
                    miniUartPuts("start=");
                    char buf[10];
                    printx32(buf, atag->mem.start);
                    miniUartPuts(buf);
                    miniUartPuts(" size=");
                    printx32(buf, atag->mem.size);
                    miniUartPuts(buf);
                    miniUartPuts("\n");
                }
                break;
            default:
                {
                    char buf[10];
                    printx32(buf, atag->hdr.tag);
                    miniUartPuts(buf);
                    miniUartPuts(" (unknown)\n");
                }
                break;
        }

        atag = (struct AtagEntry*)((uint32_t*)(atag) + atag->hdr.size);
    }
}

// }}}


int main() {
    miniUartInitialize();
    miniUartPuts("This works?\n");

    // at boot x0 will hold the ATAG addr
    walkAtags((void*)0x100);

    return 0;
}

// vim:et:ts=4

