//
// raspi3-bare-metal - Toy project for personal education
// Copright (C) 2019 Johannes Stoelp <johannes.stoelp@gmail.com>
//

.section .text.boot , "x"
_reset:
    // Halt all CPUs except cpu0.
    bl get_cpu_id
    cbz x0, 2f
1:  wfe
    b 1b

    // Setup stack in front of loader code section.
2:  ldr x0, =_reset
    mov sp, x0
    bl main
    b 1b

get_cpu_id:
    mrs x0, mpidr_el1
    and x0, x0, #3
    ret

// vim:et:ts=4

