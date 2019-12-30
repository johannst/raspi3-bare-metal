//
// raspi3-bare-metal - Toy project for personal education
// Copright (C) 2019 Johannes Stoelp <johannes.stoelp@gmail.com>
//

#pragma once

void mu_initialize();
void mu_putc(char c);
char mu_getc();
void mu_puts(const char *s);
int mu_receiver_overrun_occured(int clear_on_read);

// vim:et:ts=4

