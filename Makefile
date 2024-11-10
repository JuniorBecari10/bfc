default: compile

compile: compiler.c main.c
	gcc interpreter.c compiler.c main.c -o main -O3 -Wpedantic -std=c99
