#include <stddef.h>
#include <stdint.h>
#include "screen.h"
#include "keyboard.h"

/* read input from I/O ports */
unsigned char inportb(unsigned short _port)
{
    unsigned char rv;
    __asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
    return rv;
}

/* output to I/O ports */
void outportb(unsigned short _port, unsigned char _data)
{
    __asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}

void kernel_main(void) {
    terminal_init();
    while (1) {
        putchar(getc());
    }
}
