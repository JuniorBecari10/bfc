default: compile

compile: compiler.c main.c
	gcc interpreter.c compiler.c main.c -O3 -Wall -Wpedantic -std=c99 -o bfc
