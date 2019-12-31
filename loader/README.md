## Simple bootloader (chain loader)

This folder contains a bootloader which allows to `chain load` the real kernel
via UART into RAM.
The motivation for this bootloader is to get rid of swapping the SD card
between the development PC and the raspberry pi.

**Functional overview**
1. relocate itself to [`__loader_relocation_start`][linker_file] after being
   loaded by the `VideoCore (VC)`
1. send start sequence [`start_seq`][start_seq] via UART1 (Mini Uart) to signal
   loader ready
1. receive the kernel via the protocol described below, store kernel at `0x80000`
1. send stop sequence [`stop_seq`][stop_seq]
1. send `'y' | 'n'` char depending on success
1. jump new kernel (on success) else start again by sending out `start_seq`

**Protocol**

```
  +------+    +----------+  tx/rx  +------+
  | Host | -- | USB2Uart | ======= | RPi3 |
  +------+    +----------+         +------+

     :                send start_seq  |
     | <----------------------------- |
     |                                :
     |  kernel size in bytes          :
     |  (32bit LE word)               :
     | -----------------------------> |
     |                                :
     |  send kernel byte by byte      :
     | -----------------------------> |
     |                                | /// store incoiming byte stream
     |                 send stop_seq  | /// starting at 0x80000
     | <----------------------------- |
     :                                |
     :           send status 'y'/'n'  |
     | <----------------------------- |
     :                                | /// jump to 0x80000
     :                                |
```

[linker_file]: loader.ld
[start_seq]: loader.c#13
[stop_seq]: loader.c#14

