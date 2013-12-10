#include <stddef.h>
#include <stdint.h>
#include "screen.h"

void kernel_main(void) {
    terminal_init();
    printf("%s", "Hello World!\n");
}
