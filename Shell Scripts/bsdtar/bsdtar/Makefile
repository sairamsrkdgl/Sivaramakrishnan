# makefile for BSDTAR program, Steve McConnel, 20-Dec-90
CC=gcc -g
CFLAGS=-Wall
bsdtar: bsdtar.c
	$(CC) $(CFLAGS) -o bsdtar bsdtar.c

SHIPFILES=COPYING Makefile README bsdtar.1 bsdtar.c
bsdtar.shar: $(SHIPFILES)
	shar -a -pX $(SHIPFILES) >bsdtar.shar
