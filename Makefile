#
# raspi3-bare-metal - Toy project for personal education
# Copright (C) 2019 Johannes Stoelp <johannes.stoelp@gmail.com>
#

PREFIX := aarch64-elf-
export CC := $(PREFIX)gcc
export AS := $(PREFIX)gcc
export LD := $(PREFIX)ld
export OC := $(PREFIX)objcopy
export AR := $(PREFIX)ar

CFLAGS += -O2
#CFLAGS += -g -O0
CFLAGS += -I$(PWD)
CFLAGS += -DRPI3
CFLAGS += --std=c11
CFLAGS += -Wall -Wextra -Werror
CFLAGS += -nostdlib -nostartfiles
export CFLAGS

LDFLAGS += -nostdlib -nostartfiles -L$(PWD)/driver
export LDFLAGS

M ?= kernel
MODULE := $(M)
ifdef GDB
   # -gdb tcp::1234  -> open GDB stub on tcp port 1234
   # -S              -> freeze cpu at startup
   QEMUFLAGS = -S -gdb tcp::1234
endif

default: loader kernel

help:
	@echo "Targets:"
	@echo " kernel ................... (default) build the kernel (binary + ELF)."
	@echo " loader ................... (default) build the chain loader (binary + ELF)."
	@echo " qemu [M=<mod>] [GDB=1] ... run the moduleimage with QEMU with UART1 on stdio."
	@echo "                            M=<kernel | loader> select which image to run (default: kernel)."
	@echo "                            GDB=1 enable gdbserver in QEMU and freeze CPU until debugger attached."
	@echo " vpi3 [M=<mod>] ........... Start a virtual raspberry pi3 debug environment."
	@echo "                            This creates a tmux session with 4 splits:"
	@echo "                              - QEMU running the kernel"
	@echo "                              - Uart0 (ARM PL011)"
	@echo "                              - Uart1 (MiniUart)"
	@echo "                              - GDB attached to QEMU"
	@echo "                            M=<kernel | loader> select which image to run (default: kernel)."

loader: driver
	make -C loader

kernel: driver
	make -C kernel

driver:
	make -C driver

clean:
	make -C driver clean
	make -C loader clean
	make -C kernel clean

-include *.d

### debugging

check_module:
	@if test ! -d $(MODULE) || test ! -f $(MODULE)/kernel8.img; then    \
		echo "FAIL: $(MODULE) or $(MODULE)/kernel8.img not found!";     \
		exit 1;                                                         \
	fi

qemu: check_module
	@# -serial null -serial stdio    -> enable UART1
	qemu-system-aarch64 $(QEMUFLAGS) -machine raspi3 -kernel $(MODULE)/kernel8.img -serial null -serial stdio

vpi3: check_module
	cp $(MODULE)/kernel8.* vpi3/
	make -C vpi3 run clean

.PHONY: driver loader vpi3

# vim:noet:ts=4

