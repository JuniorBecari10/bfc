#include <stdio.h>
#include <stdlib.h>

#define LOOP_LENGTH 1000

void interpret(char *code, int len, FILE *out, int array_size, unsigned char is_signed) {
    char memory[array_size];
    char *ptr = memory;

    char *loop_stack[LOOP_LENGTH];
    int loop_depth = 0;

    for (char *c = code; c < code + len; c++) {
        switch (*c) {
            case '>':
                ptr++;

                if (ptr > memory + array_size) {
                    fprintf(stderr, "Pointer has overflown memory.");
                    return;
                }

                break;

            case '<':
                ptr--;

                if (ptr < memory) {
                    fprintf(stderr, "Pointer has underflown memory.");
                    return;
                }

                break;

            case '+': // TODO: check for overflow and underflow
                (*ptr)++;
                break;

            case '-':
                (*ptr)--;
                break;

            case '.':
                putc(*ptr, out);
                break;

            case ',':
                *ptr = getchar();
                break;

            case '[':
                if (*ptr == 0) {
                    int loop_count = 1;
                    while (loop_count > 0) {
                        c++;
                        if (*c == '[') loop_count++;
                        if (*c == ']') loop_count--;
                    }
                } else {
                    if (loop_depth >= 1000) {
                        fprintf(stderr, "Overflowed loop stack (limit: 1000).\n");
                        return;
                    }

                    loop_stack[loop_depth++] = c;
                }
                break;

            case ']':
                if (*ptr != 0)
                    c = loop_stack[loop_depth - 1];
                else
                    loop_depth--;
                break;
        }
    }
}
