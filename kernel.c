#include <stddef.h>
#include <stdint.h>
#include "screen.h"

void kernel_main(void) {
    terminal_init();
    char c = 0x41;
    
    uint32_t x = 123456789;
    
    while (1) {
        for (uint8_t k = 0; k < 80; k++) {
            if (k%2 == 0) putchar(0x20);
            else putchar(c+(x%26));
            x = 16807*x;
        }
        volatile uint32_t x = 1;
        while (x < 4000000) { x++; }
    }        
}
