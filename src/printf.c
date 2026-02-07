#include "stdio.h"
#include <stdarg.h> // variable args

void serial_write_char(char c);
void serial_write_str(const char* c);

void putc(char c) {
    serial_write_char(c);
}


void puts(const char* str) {
    while(*str) {
        putc(*str);
        str++;
    }
}

// Convert integer to string
void xint(int n, int base) {
    char buffer[32];
    int i = 0;
    int sign = 0;

    // Handle 0 explicitly
    if (n == 0) {
        putc('0');
        return;
    }

    // Handle negative numbers for Decimal (base 10)
    if (n < 0 && base == 10) {
        sign = 1;
        n = -n;
    }

    // Loop: extract digits in reverse order
    while ((unsigned int)n > 0) {
        int remainder = (unsigned int)n % base;
        if (remainder < 10) {
            buffer[i] = remainder + '0';
        } else {
            buffer[i] = (remainder - 10) + 'a'; // Hex digits a-f
        }
        n = (unsigned int)n / base;
        i++;
    }

    // If it was negative, append '-'
    if (sign) {
        buffer[i] = '-';
        i++;
    }

    // Print the buffer in reverse
    while (i > 0) {
        i--;
        putc(buffer[i]);
    }
}


void printf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    while (*fmt) {
        // If it's not '%', just print the character
        if (*fmt != '%') {
            putc(*fmt);
            fmt++;
            continue;
        }

        // If it IS '%', check the next character
        fmt++; 

        switch (*fmt) {
            case 'c': {
                // 'char' is promoted to 'int' in varargs
                char c = (char)va_arg(args, int);
                putc(c);
                break;
            }
            case 's': {
                const char* s = va_arg(args, const char*);
                puts(s);
                break;
            }
            case 'd': {
                int d = va_arg(args, int);
                xint(d, 10);
                break;
            }
            case 'x': {
                int x = va_arg(args, int);
                putc('0'); putc('x'); 
                xint(x, 16);
                break;
            }
            case '%': {
                putc('%');
                break;
            }
            default: {
                // Unknown specifier, just print it literally
                putc('%');
                putc(*fmt);
                break;
            }
        }
        fmt++;
    }

    va_end(args);
}