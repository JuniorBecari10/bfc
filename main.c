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

static int handle_args(int argc, char *argv[], int *memory_size, unsigned char *is_signed, char **in, char **out, Mode *mode);
static char *read(int *len, FILE *stream);

int main(int argc, char *argv[]) {
    // Default values
    int memory_size = 10000;
    unsigned char is_signed = 0;

    char *in = "*stdin";
    char *out = "out.c";

    Mode mode = MODE_COMPILE;

    if (handle_args(argc, argv, &memory_size, &is_signed, &in, &out, &mode) != 0)
        return 0;

    FILE *in_stream = strcmp(in, "*stdin") == 0
        ? stdin
        : fopen(in, "rb");

    if (in_stream == NULL) {
        fprintf(stderr, "Failed to open input file: '%s'\n", in);
        return 1;
    }

    int len = 0;
    char *code = read(&len, in_stream);

    if (code == NULL) {
        if (in_stream != stdin || in_stream != NULL)
            fclose(in_stream);
        
        return 1;
    }

    FILE *out_stream = strcmp(out, "*stdout") == 0
        ? stdout
        : fopen(out, "w");
    
    if (out_stream == NULL) {
        fprintf(stderr, "Failed to open output file: '%s'\n", out);
        
        if (in_stream && in_stream != stdin)
            fclose(in_stream);
        
        free(code);
        return 1;
    }

    if (!out_stream) {
        fprintf(stderr, "Failed to open output file");

        if (in_stream != stdin || in_stream != NULL)
            fclose(in_stream);
        
        free(code);
        return 1;
    }

    switch (mode) {
        case MODE_COMPILE:
            compile_c(code, len, out_stream, memory_size, is_signed);
            break;
        
        case MODE_WRITE:
            write(code, len, out_stream);
            break;
        
        case MODE_INTERPRET:
            interpret(code, len, out_stream, memory_size, is_signed);
            break;
    }

    if (in_stream && in_stream != stdin)
        fclose(in_stream);
    
    if (out_stream && out_stream != stdout)
        fclose(out_stream);
    
    free(code);
    return 0;
}

static int handle_args(int argc, char *argv[], int *memory_size, unsigned char *is_signed, char **in, char **out, Mode *mode) {
    if (argc < 2) {
        fprintf(stderr, "Expected mode. Accepted modes: 'compile', 'write', 'interpret'.\n");
        return 1;
    }

    if (strcmp(argv[1], "compile") == 0) {
        *mode = MODE_COMPILE;
        *out = "out.c";
    }
    
    else if (strcmp(argv[1], "write") == 0) {
        *mode = MODE_WRITE;
        *out = "out.bf";
    }

    else if (strcmp(argv[1], "interpret") == 0) {
        *mode = MODE_INTERPRET;
        *out = "*stdout";
    }

    else {
        fprintf(stderr, "Unknown mode: '%s'.\n", argv[1]);
        return 1;
    }
    
    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--signed") == 0)
            *is_signed = 1;
        
        else if (strcmp(argv[i], "-u") == 0 || strcmp(argv[i], "--unsigned") == 0)
            *is_signed = 0;
        
        else if (strcmp(argv[i], "-m") == 0 || strcmp(argv[i], "--memory-size") == 0) {
            if (i + 1 < argc) {
                *memory_size = atoi(argv[i + 1]);
                i++;
            }
            else {
                fprintf(stderr, "Expected memory size.\n");
                return 1;
            }
        }
        
        else if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--input") == 0) {
            if (i + 1 < argc) {
                *in = argv[i + 1];
                i++;
            }
            else {
                printf("Expected input file name, or '*stdin' for stdin.\n");
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

static char *read(int *len, FILE *stream) {
    char *code = NULL;
    size_t size = 0;

    int c;
    while ((c = fgetc(stream)) != EOF) {
        char *new_code = realloc(code, size + 2);

        if (new_code == NULL) {
            fprintf(stderr, "Cannot allocate memory.\n");

            free(code);
            return NULL;
        }

        code = new_code;
        code[size++] = (char)c;
    }

    if (code != NULL) {
        code[size] = '\0';
        *len = (int)size;
    }
    
    return code;
}
