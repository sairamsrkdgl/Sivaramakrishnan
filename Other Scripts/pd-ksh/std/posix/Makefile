# POSIX P1003.1 compatability
# does not requires SVID/P1003.2-compatible "make"

# $Header: Makefile,v 1.1 88/03/29 18:28:38 egisin Locked $

SYSTEM=SYSV
#CC = gcc -ansi -O -W 
CC = cc -O
LN = ln
PRINT = lpr -p -Plp26_3018

CFLAGS = -I../h -D_$(SYSTEM) 

MISC =	Makefile 
HDRS =	wait.h times.h unistd.h fcntl.h dirent.h 
SRCS =	unistd.c fcntl.c times.c 
OBJS =	unistd.o fcntl.o times.o 
#OBJS =	$(SRCS:.c=.o)
LIB =	libposix.a
INCL =	../h

all:	$(LIB)

link:	$(HDRS)
	[ -d $(INCL) ] || mkdir $(INCL)
	[ -d $(INCL)/sys ] || mkdir $(INCL)/sys
	$(LN) wait.h times.h $(INCL)/sys
	$(LN) unistd.h $(INCL)
	if [ ! -r /usr/include/unistd.h ]; then $(LN) dirent.h $(INCL); fi

$(LIB)(%.o): %.o

%: RCS/%,v
	co %@

$(LIB):	$(OBJS)
	ar r $@ $?
#	-ranlib $@

#$(LIB): lib.a($OBJS)
#	ar rv $@ $?
#	-ranlib $@
#	rm -f $?

std_p.tar: $(MISC) $(SRCS)
	tar cf std_p.tar $(MISC) $(HDRS) $(SRCS)

print: $(MISC) $(HDRS) $(SRCS)
	$(PRINT) $(MISC) $(HDRS) $(SRCS)

Index: $(SRCS)
	ctags -x $(HDRS) $(SRCS) >Index

ci:
	ci -l $(MISC) $(HDRS) $(SRCS)

fcntl.o: fcntl.h 

times.o: times.h 

