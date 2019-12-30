Raspberry Pi 3 memory map:

```
0x3f200000   GPIO registers
0x3f215000   Global auxiliary peripheral config
0x3f215040   Mini Uart (UART1)
```

Raspberry Pi 3 pin header:

```
                            J8
                           +---+
                     ( 1)  |# o|  ( 2)--+5V
                     ( 3)  |o o|  ( 4)--+5V
                     ( 5)  |o o|  ( 6)--GND
                     ( 7)  |o o|  ( 8)-----GPIO14 (TXD0 [ALT5])
                     ( 9)  |o o|  (10)-----GPIO15 (RXD0 [ALT5])
                     (11)  |o o|  (12)
                     (13)  |o o|  (14)
                     (15)  |o o|  (16)
                     (17)  |o o|  (18)
                     (19)  |o o|  (20)
                     (21)  |o o|  (22)
                     (23)  |o o|  (24)
                     (25)  |o o|  (26)
                     (27)  |o o|  (28)
                     (29)  |o o|  (30)
                     (31)  |o o|  (32)
                     (33)  |o o|  (34)
                     (35)  |o o|  (36)
                     (37)  |o o|  (38)
                     (39)  |o o|  (40)
                           +---+
```

Reference:
- [ARM Tags (ATAG)](http://www.simtec.co.uk/products/SWLINUX/files/booting_article.html#appendix_tag_reference)

