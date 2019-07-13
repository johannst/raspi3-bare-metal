#
# raspi3-bare-metal - Toy project for personal education
# Copright (C) 2019 Johannes Stoelp <johannes.stoelp@gmail.com>
#

FROM ubuntu:18.04

MAINTAINER Johannes Stoelp <johannes.stoelp@gmail.com>

WORKDIR /pidev

RUN set -ex;                                \
    apt-get update;                         \
    apt-get upgrade -y;                     \
    apt-get install -y                      \
        wget                                \
        xz-utils                            \
        build-essential                     \
        make                                \
        exuberant-ctags                     \
        git                                 \
        vim                                 \
        tmux                                \
        # for qemu
        pkg-config                          \
        libglib2.0-dev                      \
        libpixman-1-dev                     \
        python-minimal                      \
        libpython2.7                        \
        ;                                   \
    apt-get autoremove -y;                  \
    apt-get clean -y;                       \
    rm -rf /var/lib/apt/lists/*

RUN set -ex;                                                                                                \
    mkdir toolchain;                                                                                        \
    cd toolchain;                                                                                           \
    GCC_PKG=gcc-linaro-7.4.1-2019.02-x86_64_aarch64-elf;                                                    \
    # giga one '.' per 1MB
    wget --progress=dot:giga                                                                                \
        https://releases.linaro.org/components/toolchain/binaries/latest-7/aarch64-elf/${GCC_PKG}.tar.xz;   \
    tar xf ${GCC_PKG}.tar.xz;                                                                               \
    rm -f ${GCC_PKG}.tar.xz;                                                                                \
    echo "PATH=/pidev/toolchain/${GCC_PKG}/bin:\$PATH" >> ~/.bashrc;                                        \
    cd ..

# build qemu to get raspi3 machine
RUN set -ex;                                                                \
    git clone git://git.qemu.org/qemu.git;                                  \
    cd qemu;                                                                \
    ./configure --target-list=aarch64-softmmu --python=/usr/bin/python2.7;  \
    make -j4;                                                               \
    make install;                                                           \
    cd ..;                                                                  \
    rm -rf qemu

RUN set -ex;                                                                \
    git clone https://github.com/johannst/dotfiles;                         \
    cd dotfiles;                                                            \
    sed -i 's/n - \(gitconfig\)/y - \1/' install.config;                    \
    sed -i 's/n - \(gdb\)/y - \1/' install.config;                          \
    ./install -y;                                                           \
    cd ..

RUN set -ex;                                                                \
    git clone https://github.com/johannst/raspi3-bare-metal

CMD cd raspi3-bare-metal && git pull; /bin/bash -l

# vim:expandtab

