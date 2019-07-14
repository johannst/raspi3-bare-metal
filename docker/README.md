## Docker development box

This `Dockerfile` can be used to build a docker image to directly start
developing, running and debugging the bare metal software in this repository.

It contains:
- `aarch64-elf` gcc toolchain
- `aarch64-elf` gdb debugger
- `qemu-system-aarch64` with `-machine raspi3` for emulation

### Building the image

The local build script can be used to build the image (this will take some time
depending on the machine):
```
> ./build.sh
...
[+] SUCCESS took 1147s

> docker images
REPOSITORY          TAG                 IMAGE ID            CREATED             SIZE
rpi3devbox          latest              1136c6fce8a7        25 minutes ago      1.07GB
```

### Running the image

To create a container simply run:
```
# -it   interactive with pseudo tty
# --rm  remove container after exiting
> docker run -it --rm rpi3devbox
```

To additionally map a serial device from the host pc into the container:
```
docker run --device=/dev/ttyUSB0 --cap-add=SYS_ADMIN -it --rm rpi3devbox
```

[//]: # vim:colorcolumn=80:textwidth=80:expandtab:shiftwidth=2:softtabstop=2:

