#include <stddef.h>
#include <stdint.h>

#define COLUMNS 80
#define LINES 24

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

uint8_t make_vga_color(enum vga_color fg_color, enum vga_color bg_color);
uint16_t make_vga_entry(char c, uint8_t color);
char getcharat(uint8_t x, uint8_t y);
void putcharat(char c, uint8_t x, uint8_t y);
void putchar(char c);
size_t printf(char* fmt, ...);
void terminal_scroll(void);
void terminal_clear(void);
void terminal_init(void);
