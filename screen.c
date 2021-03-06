#include <stdarg.h>
#include "screen.h"
#include "motd.h"

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

char getcharat(uint8_t x, uint8_t y)
{
    const uint16_t index = y * COLUMNS + x;
    return (char)terminal_buffer[index];
}

void putcharat(char c, uint8_t x, uint8_t y)
{
    const uint16_t index = y * COLUMNS + x;
    terminal_buffer[index] = make_vga_entry(c, terminal_color);
}

void putchar(char c)
{
    switch(c) {
    case 0:
        break;
    case 8:   // backspace
        if (terminal_column != 0) {
            terminal_column--;
        } else if (terminal_row != 0) {
            terminal_column = COLUMNS - 1;
            terminal_row--;
            while (getcharat(terminal_column, terminal_row) == '\0') {
                if (terminal_column == 0) {
                    if (terminal_row > 0) {
                        terminal_row--;
                    } else {
                        break;
                    }
                } else {
                    terminal_column--;
                }
            }
        }
        putcharat('\0', terminal_column, terminal_row);
        break;
    case 9:   // horiz tab
        terminal_row += 8;
        if (terminal_row >= LINES) {
            terminal_row = 0;
        }
        break;
    case 10:  // newline
        while (terminal_column < COLUMNS) {
            putcharat('\0', terminal_column++, terminal_row);
        }
        terminal_column = 0;
        if (++terminal_row == LINES) {
            terminal_row--;
            terminal_scroll();
        }
        break;
    default:
        putcharat(c, terminal_column, terminal_row);
        if (++terminal_column == COLUMNS) {
            terminal_column = 0;
            if (++terminal_row == LINES) {
                terminal_row--;
                terminal_scroll();
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

void terminal_scroll(void)
{
    uint16_t index;
    for (uint8_t y = 0; y < LINES-1; y++) {
        for (uint8_t x = 0; x < COLUMNS; x++) {
            index = y * COLUMNS + x;
            terminal_buffer[index] = terminal_buffer[index+COLUMNS];
        }
    }
    uint16_t blank = make_vga_entry('\0', terminal_color);
    for (uint8_t x = 0; x < COLUMNS; x++) {
        index = (LINES - 1) * COLUMNS + x;
        terminal_buffer[index] = blank;
    }
}

void terminal_clear(void)
{
    uint16_t c = make_vga_entry('\0', terminal_color);
    for (size_t k = 0; k < COLUMNS*LINES; k++) {
        terminal_buffer[k] = c;
    }
}

void terminal_init(void)
{
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = make_vga_color(COLOR_WHITE, COLOR_BLACK);
    terminal_buffer = (uint16_t*) 0xB8000;
    terminal_clear();
}
