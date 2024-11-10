#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    MODE_COMPILE,
    MODE_
} Mode;

void compile_c(char *code, int len, FILE *out, int array_size, unsigned char is_signed);

int main(int argc, char *argv[]) {
    // Default values
    int array_size = 10000;
    unsigned char is_signed = 0;
    char code[1000];
    int len = 0;
    char *out = "out.c";

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--signed") == 0) {
            is_signed = 1;
        }
        
        else if (strcmp(argv[i], "-u") == 0 || strcmp(argv[i], "--unsigned") == 0) {
            is_signed = 0;
        }
        
        else if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--array-size") == 0) {
            if (i + 1 < argc) {
                array_size = atoi(argv[i + 1]);
                i++;
            }
            else {
                printf("Expected array size.\n");
                return 1;
            }
        }
        
        else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0) {
            if (i + 1 < argc) {
                out = argv[i + 1];
                i++;
            }
            else {
                printf("Expected output file name, or '*stdout' for stdout.\n");
                return 1;
            }
        }
    }

    while (fgets(code + len, sizeof(code) - len, stdin) != NULL) {
        len = strlen(code);
        if (code[len - 1] == '\n') {
            code[len - 1] = '\0';
            break;
        }
    }

    FILE *out_stream = strcmp(out, "*stdout") == 0
        ? stdout
        : fopen(out, "w");
    
    if (!out_stream) {
        perror("Failed to open output file");
        return 1;
    }

    compile_c(code, len, out_stream, array_size, is_signed);
    
    if (out_stream != stdout)
        fclose(out_stream);
    
    return 0;
}

void compile_c(char *code, int len, FILE *out, int array_size, unsigned char is_signed) {
    #define REPEAT(ch)               \
        do {                         \
            while (*(c + 1) == ch) { \
                    count++;         \
                    c++;             \
                }                    \
        } while (0)

    int indent_level = 1;

    int open_brackets[1000] = {0};
    int *open_index = open_brackets;

    fprintf(out, "#include <stdio.h>\n\n");
    fprintf(out, "int main(void) {\n");
    fprintf(out, "    unsigned char memory[%d] = {0};\n", array_size);
    fprintf(out, "    %schar *ptr = memory;\n", (is_signed ? "" : "unsigned "));

    for (char *c = code; c < code + len; c++) {
        for (int i = 0; i < (*c == ']' ? indent_level - 1 : indent_level); i++) {
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

    fprintf(out, "return 0;\n");
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
