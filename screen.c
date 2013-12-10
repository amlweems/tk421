#include <stdarg.h>
#include "screen.h"

const size_t COLUMNS = 80;
const size_t LINES  = 24;

uint8_t make_vga_color(enum vga_color fg_color, enum vga_color bg_color)
{
    return fg_color | bg_color << 4;
}

uint16_t make_vga_entry(char c, uint8_t color)
{
    uint16_t c16 = c;
    uint16_t color16 = color;
    return c16 | (color16 << 8);
}

void putcharat(char c, uint8_t x, uint8_t y)
{
    const uint8_t index = y * COLUMNS + x;
    terminal_buffer[index] = make_vga_entry(c, terminal_color);
}

void putchar(char c)
{
    switch(c) {
    case 8:   // backspace
        if (terminal_row != 0) {
            terminal_row--;
        }
        break;
    case 9:   // horiz tab
        terminal_row += 8;
        if (terminal_row >= LINES) {
            terminal_row = 0;
        }
        break;
    case 10:  // newline
        terminal_column = 0;
        if (++terminal_row == LINES) {
            terminal_row = 0;
        }
        break;
    default:
        putcharat(c, terminal_column, terminal_row);
        if (++terminal_column == COLUMNS) {
            terminal_column = 0;
            if (++terminal_row == LINES) {
                terminal_row = 0;
            }
        }
    }
}

size_t printf(char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    size_t len = 0;
    uint8_t format = 0;

    while (*fmt != '\0') {
        if (format) {
            format = 0;
            if (*fmt == '%') {
                putchar('%');
                len++;
            } else if (*fmt == 'd') {
                uint32_t v = va_arg(args, uint32_t);
                size_t k = 0;
                char buffer[10] = { 0 };
                while (v) {
                    buffer[k++] = 0x30 + (v%10);
                    v /= 10;
                }
                while (k > 0) {
                    putchar(buffer[--k]);
                    len++;
                }
            } else if (*fmt == 'c') {
                char v = va_arg(args, int);
                putchar(v);
                len++;
            } else if (*fmt == 's') {
                char* v = va_arg(args, char*);
                while (*v != '\0') {
                    putchar(*v);
                    len++;
                    v++;
                }
            }
        } else {
            if (*fmt == '%') {
                format = 1;
            } else {
                putchar(*fmt);
                len++;
            }
        }
        fmt++;
    }
    va_end(args);
    return len;
}

void terminal_init(void)
{
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = make_vga_color(COLOR_WHITE, COLOR_BLACK);
    terminal_buffer = (uint16_t*) 0xB8000;
}