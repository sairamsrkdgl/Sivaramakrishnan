# Standard C (ANSI) compatabilaty
# does not requires SVID/P1003.2-compatible "make"

# $Header: Makefile,v 1.1 88/03/29 18:28:38 egisin Locked $

SYSTEM=SYSV
CC = cc -O #haa
#CC = gcc -ansi -O -W 
LN = ln
PRINT = lpr -p -Plp26_3018

CFLAGS = -I../h -D_$(SYSTEM)

MISC =	Makefile stdio.h_std
HDRS =	limits.h stddef.h stdlib.h string.h time.h stdarg.h 
SRCS =	clock.c stdio.c setvbuf.c vprintf.c fprintf.c sprintf.c \
	strstr.c \
	memmove.c memcpy.c memset.c memcmp.c memchr.c 
OBJS =	clock.o stdio.o setvbuf.o vprintf.o fprintf.o sprintf.o \
	strstr.o \
	memmove.o memcpy.o memset.o memcmp.o memchr.o 
#OBJS =	$(SRCS:.c=.o)
LIB =	libstdc.a
INCL =	../h

all:	$(LIB)

link:	$(HDRS) stdio.h 
	[ -d $(INCL) ] || mkdir $(INCL)
	[ -d $(INCL)/sys ] || mkdir $(INCL)/sys
	$(LN) types.h $(INCL)/sys
	$(LN) limits.h stddef.h stdlib.h stdio.h string.h time.h stdarg.h $(INCL)

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

stdio.h: stdio.h_std /usr/include/stdio.h 
	sed </usr/include/stdio.h >stdio.hacked -e '/^# *include/ d' -e '/char..sprintf/ d' -e '/# *define.NULL./ d'
	sed <stdio.h_std >stdio.h -e '/%%%/ r stdio.hacked'
	rm stdio.hacked

std_c.tar: $(MISC) $(HDRS) $(SRCS)
	tar cf std_c.tar $(MISC) $(HDRS) $(SRCS)

print: $(MISC) $(HDRS) $(SRCS)
	$(PRINT) $(MISC) $(HDRS) $(SRCS)

string.h: stddef.h

stdlib.h: stddef.h

stdio.h: stddef.h 

time.h: stddef.h

stdio.o: stdio.h

setvbuf.o: stdlib.h stdio.h

fprintf.o: stdarg.h stdio.h

sprintf.o: stdarg.h stdio.h

vprintf.o: stdarg.h stdio.h

strstr.o: string.h 

