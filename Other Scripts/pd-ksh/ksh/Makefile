# PD Bourne/Korn Shell

# $Header: Makefile,v 3.2 88/11/06 11:34:12 egisin Exp $

BIN = .
#BIN = /u/egisin/bin
STD = ../std
PRINT = lpr -p -Plp26_3018

# CC = cc
CC = cc 
#CC = gcc 

# Must define one of _V7, _SYSV, _BSD, _POSIX
# may define any of JOBS (have BSD or POSIX job control),
# EDIT (emacs-like command editing), SILLY (see edit.c)

OPTIONS = -D_SYSV -DEDIT -DSILLY
#OPTIONS = -D_BSD -DJOBS -DEDIT 

CFWARN = -O
#CFWARN = -ansi -O -W -Wcomment # -Wreturn-type
CFLAGS = $(CFWARN) -I$(STD)/h $(OPTIONS) $(JUNK) 
LDFLAGS = -L$(STD) $(JUNK) 
LDLIBS = -lstdc -lposix			# compatability libraries

HDRS =	sh.h table.h expand.h lex.h tree.h tty.h
SRCS1 =	version.c main.c misc.c trap.c alloc.c io.c \
	syn.c lex.c edit.c history.c tree.c 
SRCS2 =	exec.c jobs.c \
	c_sh.c c_ksh.c c_test.c getopts.c ulimit.c \
	var.c table.c eval.c expr.c 
SRCS =	Makefile $(HDRS) $(SRCS1) $(SRCS2) 

OBJS =	version.o main.o misc.o \
	syn.o lex.o edit.o tree.o \
	exec.o jobs.o trap.o \
	c_sh.o c_ksh.o c_test.o \
	ulimit.o getopts.o expr.o history.o \
	var.o table.o alloc.o io.o eval.o 

ksh: $(OBJS) 
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

install: ksh
	cp ksh $(BIN)/ksh

print: Index $(SRCS)
	$(PRINT) Index $(HDRS)
	$(PRINT) $(SRCS1) 
	$(PRINT) $(SRCS2) 

Index: $(SRCS)
	ctags -x $(SRCS) >Index

ci:
	touch version.c
	ci -u -q $(SRCS)

fix_csh:
	rm -f /bin/csh

tar:	ReadMe ksh.1 $(SRCS)
	tar cf /tmp/egisin/ksh.tar ReadMe ksh.1 $(SRCS)

