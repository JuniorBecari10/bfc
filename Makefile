default: compile

compile: compiler.c main.c
	gcc compiler.c main.c -o main
