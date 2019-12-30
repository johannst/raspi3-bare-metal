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

    // Setup stack in front of loader code section (after relocation).
2:  ldr x0, =_reset
    mov sp, x0

    // Relocate loader from 0x80000 (addr we get loaded by VC) to
    // __loader_relocation_start by copying quadword by quadword.
    ldr x0, =0x80000
    ldr x1, =__loader_relocation_start
    ldr x2, =__loader_size
1:  ldr x3, [x0], 8
    str x3, [x1], 8
    sub x2, x2, 1
    cbnz x2, 1b

    // Jump to loader main().
    // The branch is specifically crafted using ldr/blr because this is
    // position independent code (PIC), which is important since we relocated
    // ourself.
    ldr x0, =main
    blr x0
    b 1b

get_cpu_id:
    mrs x0, mpidr_el1
    and x0, x0, #3
    ret

// vim:et:ts=4

