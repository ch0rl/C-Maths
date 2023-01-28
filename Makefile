CC = gcc
CFLAGS = -lm -O3

build: src/Maths.c
	${CC} ${CFLAGS} src/Maths.c -o Maths

assembly:
	${CC} -S -masm=intel -Og -fverbose-asm src/Maths.c -o Maths.nasm 
