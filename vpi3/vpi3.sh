#!/bin/bash
#
# raspi3-bare-metal - Toy project for personal education
# Copright (C) 2019 Johannes Stoelp <johannes.stoelp@gmail.com>
#

## check old session
if tmux list-session 2> /dev/null | grep vpi3 >& /dev/null; then
    echo -en '[+] session already running, kill it? [yYnN]\n> '
    read kill_it
    [[ $kill_it =~ ^[^yY]$ ]] && {
        echo -e '[+] keeping old session, to attach run\n> tmux a -t vpi3'
        exit 0
    }
    tmux kill-session -t vpi3
fi

## create panes
#  +-----------+-----------+
#  |    0:0    |           |
#  +-----------+           |
#  |           |           |
#  |    0:1    |           |
#  |           |    0:3    |
#  +-----------+           |
#  |           |           |
#  |    0:2    |           |
#  |           |           |
#  +-----------+-----------+
tmux new -d -s vpi3
tmux split-window -t vpi3:0.0 -h
tmux split-window -t vpi3:0.0 -v -p 80
tmux split-window -t vpi3:0.1 -v -p 50

## global opts
tmux set -t vpi3 mouse on

## setup panes
tmux send-keys -t vpi3:0.1 "tty > UART0; \
                            clear; \
                            echo -e '### ARM PL011 UART\n'; \
                            bash /dev/ptmx" C-m
tmux send-keys -t vpi3:0.2 "tty > UART1; \
                            clear; \
                            echo -e '### MiniUart\n'; \
                            bash /dev/ptmx" C-m

qemu_cmd='qemu-system-aarch64 \
          -display none \
          -M raspi3 \
          -kernel kernel8.img \
          -serial $(cat UART0) \
          -serial $(cat UART1) \
          -S \
          -gdb tcp::1234'
gdb_cmd='aarch64-elf-gdb -q \
         -ex "target remote :1234" \
         -ex "symbol-file kernel8.elf"'

# launch simulator
sleep 0.5; tmux send-keys -t vpi3:0.0 "$qemu_cmd && tmux kill-session" C-m

# launch debugger
sleep 0.5; tmux send-keys -t vpi3:0.3 "$gdb_cmd" C-m

# set focus on debugger
tmux select-pane -t vpi3:0.3

# attach to session
tmux attach -t vpi3

