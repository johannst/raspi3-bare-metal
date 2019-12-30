#
# raspi3-bare-metal - Toy project for personal education
# Copright (C) 2019 Johannes Stoelp <johannes.stoelp@gmail.com>
#

libdriver.a: gpio.o mini_uart.o
	$(AR) rcs $@ $^

%.o: %.c
	$(CC) -o $@ $(CFLAGS) -c $<

clean:
	rm -f *.o *.d
	rm -f libdriver.a

-include *.d

# vim:noet:ts=4

