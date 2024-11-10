#include <stdio.h>
#include "compiler.h"

void compile_c(char *code, int len, FILE *out, int array_size, unsigned char is_signed) {
    #define REPEAT(ch)                  \
        do {                            \
            while (*(c + 1) == ch) {    \
                count++;                \
                c++;                    \
            }                           \
        } while (0)

    int indent_level = 1;

    int open_brackets[1000] = {0};
    int *open_index = open_brackets;

    fprintf(out, "#include <stdio.h>\n\n");
    fprintf(out, "int main(void) {\n");
    fprintf(out, "    unsigned char memory[%d] = {0};\n", array_size);
    fprintf(out, "    %schar *ptr = memory;\n", (is_signed ? "" : "unsigned "));

    for (char *c = code; c < code + len; c++) {
        for (int i = 0; i < indent_level; i++) {
            fprintf(out, "    ");
        }

        switch (*c) {
            case '>': {
                int count = 1;
                REPEAT('>');

                if (count == 1)
                    fprintf(out, "ptr++;\n");
                else
                    fprintf(out, "ptr += %d;\n", count);
                break;
            }
            
            case '<': {
                int count = 1;
                REPEAT('<');

                if (count == 1)
                    fprintf(out, "ptr--;\n");
                else
                    fprintf(out, "ptr -= %d;\n", count);
                break;
            }
            
            case '+': {
                int count = 1;
                REPEAT('+');

                if (count == 1)
                    fprintf(out, "(*ptr)++;\n");
                else
                    fprintf(out, "(*ptr) += %d;\n", count);
                break;
            }
            
            case '-': {
                int count = 1;
                REPEAT('-');

                if (count == 1)
                    fprintf(out, "(*ptr)--;\n");
                else
                    fprintf(out, "(*ptr) -= %d;\n", count);
                break;
            }
            
            case '.':
                fprintf(out, "putchar(*ptr);\n");
                break;
            
            case ',':
                fprintf(out, "*ptr = getchar();\n");
                break;
            
            case '[':
                fprintf(out, "while (*ptr) {\n");
                indent_level++;
                *open_index = (c - code);
                open_index++;
                break;

            case ']':
                fprintf(out, "}\n");
                indent_level--;
                open_index--;
                break;
        }
    }

    fprintf(out, "}\n");

    if (open_index > open_brackets) {
        fprintf(stderr, "Error - unmatched brackets at position(s): ");

        for (int *b = open_brackets; b < open_index; b++) {
            fprintf(stderr, "%d ", *b);
        }

        fprintf(stderr, "\n");
    }

    #undef REPEAT
}

void write(char *msg, int len, FILE *out) {
    int count = 0;

    for (char *c = msg; c < msg + len; c++) {
        while (count != *c) {
            if (count < *c) {
                fprintf(out, "+");
                count++;
            }

            else if (count > *c) {
                fprintf(out, "-");
                count--;
            }
        }

        fprintf(out, ".");
    }
}
