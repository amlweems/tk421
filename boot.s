; declare flags and magic for Multiboot header
MBALIGN     equ  1<<0
MEMINFO     equ  1<<1
FLAGS       equ  MBALIGN | MEMINFO
MAGIC       equ  0x1BADB002
CHECKSUM    equ  -(MAGIC + FLAGS)

; create the Multiboot header
segment .multiboot
align 4
dd    MAGIC
dd    FLAGS
dd    CHECKSUM

segment .bootstrap_stack
stack_bottom:
times 16384 db 0
stack_top:

segment .text
global _start
_start:
    ; set up the stack (grows downwards)
    mov   esp, stack_top

    ; call our kernel
    extern kernel_main
    call  kernel_main

    ; in the case that the kernel returns, disable
    ; interrupts and halt the processor
    cli
.hang:
    hlt
    jmp   .hang
