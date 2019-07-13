#!/bin/bash
#
# raspi3-bare-metal - Toy project for personal education
# Copright (C) 2019 Johannes Stoelp <johannes.stoelp@gmail.com>
#

image_tag=rpi3devbox

green() { echo -e "\e[32m${@}\e[0m"; }
red() { echo -e "\e[31m${@}\e[0m"; }

echo "[+] Starting to build $(green $image_tag)"

SECONDS=0
if docker build --tag $image_tag .; then
    echo "[+] $(green SUCCESS) took ${SECONDS}s"
else
    echo "[-] $(red FAILED) took ${SECONDS}s"
fi

