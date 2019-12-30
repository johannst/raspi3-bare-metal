//
// raspi3-bare-metal - Toy project for personal education
// Copright (C) 2019 Johannes Stoelp <johannes.stoelp@gmail.com>
//

#include "driver/mini_uart.h"
#include <stdint.h>

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

struct AtagCmdline {
        char cmdline[1]; // min size, single string
};

struct AtagEntry {
    struct AtagHeader hdr;
    union {
        struct AtagCore core;
        struct AtagMem mem;
        struct AtagCmdline cmd;
    };
};

enum eAtagType {
    ATAG_NONE = 0x00000000,
    ATAG_CORE = 0x54410001,
    ATAG_MEM  = 0x54410002,
    ATAG_CMDLINE = 0x54410009
};

void walkAtags(void* start_addr) {
    struct AtagEntry* atag = (struct AtagEntry*)start_addr;

    while (atag->hdr.tag != ATAG_NONE) {
        mu_puts("Found ATAG tag=");
        switch (atag->hdr.tag) {
            case ATAG_CORE:
                mu_puts("ATAG_CORE\n");
                break;
            case ATAG_MEM:
                mu_puts("ATAG_MEM\n");
                {
                    mu_puts("start=0x");
                    char buf[10];
                    printx32(buf, atag->mem.start);
                    mu_puts(buf);
                    mu_puts(" size=0x");
                    printx32(buf, atag->mem.size);
                    mu_puts(buf);
                    mu_puts("\n");
                }
                break;
	    case ATAG_CMDLINE:
                mu_puts("ATAG_CMDLINE\n");
                mu_puts(atag->cmd.cmdline);
		break;
            default:
                {
                    char buf[10];
                    printx32(buf, atag->hdr.tag);
                    mu_puts(buf);
                    mu_puts(" (unknown)\n");
                }
                break;
        }

        atag = (struct AtagEntry*)((uint32_t*)(atag) + atag->hdr.size);
    }
}

// }}}

int main() {
    mu_initialize();
    mu_puts("This works?\n");

    // at boot x0 will hold the ATAG addr
    walkAtags((void*)0x100);

    while (1) {
        char c = mu_getc();
        mu_puts("got: ");
        mu_putc(c);
        //mu_putc('\n');
    }

    return 0;
}

// vim:et:ts=4

