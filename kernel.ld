/*                                                                  */
/* raspi3-bare-metal - Toy project for personal education           */
/* Copright (C) 2019 Johannes Stoelp <johannes.stoelp@gmail.com>    */
/*                                                                  */

SECTIONS {
   . = 0x80000;
   .text : {
      /* input_file_pattern '('section_pattern')' */
      * (.text.*);
   }
}

/* vim:set et ts=4: */

