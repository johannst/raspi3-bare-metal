#
# raspi3-bare-metal - Toy project for personal education
# Copright (C) 2019 Johannes Stoelp <johannes.stoelp@gmail.com>
#

PREFIX := aarch64-elf-
CC := $(PREFIX)gcc
AS := $(PREFIX)gcc
LD := $(PREFIX)ld
OC := $(PREFIX)objcopy

OBJS  = boot.o main.o

CFLAGS += -O2
#CFLAGS += -g -O0
CFLAGS += --std=gnu11
CFLAGS += -Wall -Wextra -Werror
CFLAGS += -nostdlib -nostartfiles
CFLAGS += -MT $@ -MMD -MP -MF $(@:%.o=%.d)

LDFLAGS += -nostdlib -nostartfiles

ifdef GDB
   # -gdb tcp::1234  -> open GDB stub on tcp port 1234
   # -S              -> freeze cpu at startup
   QEMUFLAGS = -S -gdb tcp::1234
endif

kernel8.img: kernel8.elf
	$(OC) -O binary $^ $@

kernel8.elf: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ -T kernel.ld $^

%.o: %.s
	$(AS) -o $@ $(CFLAGS) -c $<

%.o: %.c
	$(CC) -o $@ $(CFLAGS) -c $<

clean:
	rm -f *.o *.d
	rm -f kernel8.{elf,img}

-include *.d

qemu:
	@# -serial null -serial stdio    -> enable UART1
	qemu-system-aarch64 $(QEMUFLAGS) -machine raspi3 -kernel kernel8.img -serial null -serial stdio

vpi3: kernel8.img
	cp kernel8.img kernel8.elf vpi3/
	make -C vpi3 run clean

# vim:noet:ts=4

