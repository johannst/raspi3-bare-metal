//
// raspi3-bare-metal - Toy project for personal education
// Copright (C) 2019 Johannes Stoelp <johannes.stoelp@gmail.com>
//

#include "driver/mini_uart.h"
#include <stdint.h>

typedef void (*kernel_fn)();

#define KERNEL_START 0x80000

const char start_seq[] = "pi3loader start\n";
const char stop_seq[] = "pi3loader done\n";

int main() {
    mu_initialize();

receive_kernel:
    mu_puts(start_seq);
    uint32_t kernel_size = 0;
    kernel_size |= (mu_getc() << 0);
    kernel_size |= (mu_getc() << 8);
    kernel_size |= (mu_getc() << 16);
    kernel_size |= (mu_getc() << 24);

    volatile unsigned char* kernel_addr = (unsigned char*)KERNEL_START;
    while (kernel_size-- != 0) {
        *kernel_addr++ = mu_getc();
    }

    mu_puts(stop_seq);
    if (mu_receiver_overrun_occured(1 /* clear on read */)) {
        mu_putc('n');
        goto receive_kernel;
    }
    mu_putc('y');

    ((kernel_fn)KERNEL_START)();

    return 0;
}

// vim:et:ts=4

