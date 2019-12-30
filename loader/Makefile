#
# raspi3-bare-metal - Toy project for personal education
# Copright (C) 2019 Johannes Stoelp <johannes.stoelp@gmail.com>
#

CFLAGS += -MT $@ -MMD -MP -MF $(@:%.o=%.d)

OBJS = boot.o loader.o

loader: kernel8.img

kernel8.img: kernel8.elf
	$(OC) -O binary $^ $@

kernel8.elf: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ -T loader.ld $^ -ldriver

%.o: %.s
	$(AS) -o $@ $(CFLAGS) -c $<

%.o: %.c
	$(CC) -o $@ $(CFLAGS) -c $<

clean:
	rm -f *.o *.d
	rm -f kernel8.elf kernel8.img

-include *.d

.PHONY: loader

# vim:noet:ts=4

