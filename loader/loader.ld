/*
 * raspi3-bare-metal - Toy project for personal education
 * Copright (C) 2019 Johannes Stoelp <johannes.stoelp@gmail.com>
 */

SECTIONS {
   . = 0x80000 - 0x1000;
   __loader_relocation_start = .;
   .text : {
      * (.text.boot);
      * (.text*);
   }
   .rodata : {
       * (.rodata*);
   }
   .data : {
       * (.data*);
   }
   __loader_end = .;
}

/* create symbol @address == size of loader in 8byte */
__loader_size = __loader_end - __loader_relocation_start >> 3;
ASSERT(__loader_size< 0x1000, "loader code section bigger than 4K!");

/* vim:set et ts=4: */

