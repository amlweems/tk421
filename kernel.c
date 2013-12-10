#include <stddef.h>
#include <stdint.h>

#define NEWLINE 10

const size_t COLUMNS = 80;
const size_t LINES  = 24;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

enum vga_color
{
    COLOR_BLACK = 0,
    COLOR_BLUE = 1,
    COLOR_GREEN = 2,
    COLOR_CYAN = 3,
    COLOR_RED = 4,
    COLOR_MAGENTA = 5,
    COLOR_BROWN = 6,
    COLOR_LIGHT_GREY = 7,
    COLOR_DARK_GREY = 8,
    COLOR_LIGHT_BLUE = 9,
    COLOR_LIGHT_GREEN = 10,
    COLOR_LIGHT_CYAN = 11,
    COLOR_LIGHT_RED = 12,
    COLOR_LIGHT_MAGENTA = 13,
    COLOR_LIGHT_BROWN = 14,
    COLOR_WHITE = 15,
};

uint8_t make_vga_color(enum vga_color fg_color, enum vga_color bg_color) {
    return fg_color | bg_color << 4;
}

uint16_t make_vga_entry(char c, uint8_t color) {
    uint16_t c16 = c;
    uint16_t color16 = color;
    return c16 | (color16 << 8);
}

void terminal_putcharat(char c, uint8_t x, uint8_t y) {
    const uint8_t index = y * COLUMNS + x;
    terminal_buffer[index] = make_vga_entry(c, terminal_color);
}

void terminal_putchar(char c) {
	if (c == NEWLINE) {
		terminal_column = 0;
		if (++terminal_row == LINES) {
            terminal_row = 0;
        }
	} else {
		terminal_putcharat(c, terminal_column, terminal_row);
	}
    if (++terminal_column == COLUMNS) {
        terminal_column = 0;
        if (++terminal_row == LINES) {
            terminal_row = 0;
        }
    }
}

void terminal_write(char* str) {
    size_t k = 0;
    while (str[k] != '\0') {
    	terminal_putchar(str[k]);
        k++;
    }
}

void terminal_init(void) {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = make_vga_color(COLOR_WHITE, COLOR_BLACK);
    terminal_buffer = (uint16_t*) 0xB8000;
}

void test_pixels(void) {
    unsigned char *VGA = (unsigned char*)0xA0000000L;
    for (uint8_t k = 0; k < 10; k++) {
        VGA[k] = COLOR_MAGENTA;    
    }
}

void kernel_main(void) {
    terminal_init();
    terminal_write("Hello world!\n");
}
