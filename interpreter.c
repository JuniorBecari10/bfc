#include <stdio.h>
#include <stdlib.h>

void interpret(char *code, int len, FILE *out, int array_size, unsigned char is_signed) {
    char memory[array_size];
    char *ptr = memory;

    // Array to store positions of opening brackets
    char *loop_stack[1000];
    int loop_depth = 0;  // Track the depth of nested loops

    for (char *c = code; c < code + len; c++) {
        switch (*c) {
            case '>':
                ptr++;
                break;

            case '<':
                ptr--;
                break;

            case '+':
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
