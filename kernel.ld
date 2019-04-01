/* raspi3-bare-metal/Makefile */
/* johannst (c)               */

SECTIONS {
   . = 0x80000;
   .text : {
      /* input_file_pattern '('section_pattern')' */
      * (.text.*);
   }
}

/* vim:set et ts=4: */

