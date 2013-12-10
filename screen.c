#include "screen.h"

const size_t COLUMNS = 80;
const size_t LINES  = 24;

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
	if (c == '\n') {
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
