CC = gcc
CFLAGS = -lm -O3

build: src/Maths.c
	${CC} ${CFLAGS} src/Maths.c -o Maths

assembly:
	${CC} -S -masm=intel -Og -fverbose-asm src/Maths.c -o Maths.nasm 

docs:
	xelatexmk -synctex=1 -interaction=nonstopmode -file-line-error -xelatex -outdir=docs/.out -shell-escape docs/c-maths.tex
	cp docs/.out/c-maths.pdf maths.pdf
