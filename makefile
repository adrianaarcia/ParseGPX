CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -g3

ParseGPX: parse.c
	${CC} ${CFLAGS} -o $@ $^
