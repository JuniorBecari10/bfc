#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "compiler.h"
#include "interpreter.h"

typedef enum {
    MODE_COMPILE,
    MODE_WRITE,
    MODE_INTERPRET
} Mode;

static int handle_args(int argc, char *argv[], int *array_size, unsigned char *is_signed, char **out, Mode *mode);
static char *read_stdin(int *len);

int main(int argc, char *argv[]) {
    // Default values
    int array_size = 10000;
    unsigned char is_signed = 0;

    char *out = "out.c";
    Mode mode = MODE_COMPILE;

    int len = 0;
    char *code = read_stdin(&len);

    if (handle_args(argc, argv, &array_size, &is_signed, &out, &mode) != 0) {
        free(code);
        return 0;
    }

    FILE *out_stream = strcmp(out, "*stdout") == 0
        ? stdout
        : fopen(out, "w");
    
    if (!out_stream) {
        fprintf(stderr, "Failed to open output file");
        return 1;
    }

    switch (mode) {
        case MODE_COMPILE:
            compile_c(code, len, out_stream, array_size, is_signed);
            break;
        
        case MODE_WRITE:
            write(code, len, out_stream);
            break;
        
        case MODE_INTERPRET:
            interpret(code, len, array_size, is_signed);
            break;
    }
    
    if (out_stream != stdout)
        fclose(out_stream);
    
    free(code);
    return 0;
}

static int handle_args(int argc, char *argv[], int *array_size, unsigned char *is_signed, char **out, Mode *mode) {
    if (argc < 2) {
        fprintf(stderr, "Expected mode. Accepted modes: 'compile', 'write', 'interpret'.\n");
        return 1;
    }

    if (strcmp(argv[1], "compile") == 0)
        *mode = MODE_COMPILE;
    else if (strcmp(argv[1], "write") == 0) {
        *mode = MODE_WRITE;
        *out = "out.bf";
    }
    else if (strcmp(argv[1], "interpret") == 0) {
        *mode = MODE_INTERPRET;
    }
    else {
        fprintf(stderr, "Unknown mode: '%s'.\n", argv[1]);
        return 1;
    }
    
    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--signed") == 0) {
            *is_signed = 1;
        }
        
        else if (strcmp(argv[i], "-u") == 0 || strcmp(argv[i], "--unsigned") == 0) {
            *is_signed = 0;
        }
        
        else if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--array-size") == 0) {
            if (i + 1 < argc) {
                *array_size = atoi(argv[i + 1]);
                i++;
            }
            else {
                perror("Expected array size.");
                return 1;
            }
        }
        
        else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0) {
            if (i + 1 < argc) {
                *out = argv[i + 1];
                i++;
            }
            else {
                printf("Expected output file name, or '*stdout' for stdout.\n");
                return 1;
            }
        }
    }

    return 0;
}

static char *read_stdin(int *len) {
    char *code = NULL;
    size_t size = 0;

    char c;
    while ((c = fgetc(stdin)) != EOF) {
        char *new = realloc(code, size + 1);

        if (new == NULL) {
            fprintf(stderr, "Cannot allocate memory.\n");
            free(new);

            return NULL;
        }

        code = new;
        code[(*len)++] = (char)c;
        size++;
    }

    if (code != NULL)
        code[*len] = 0;
    
    fflush(stdin);
    return code;
}
