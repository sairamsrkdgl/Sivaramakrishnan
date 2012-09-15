# makefile for stat
#			by Kevin S Braunsdorf, PUCC

BIN=	${DESTDIR}/usr/unsup/bin

I=/usr/include
S=/usr/include/sys

INCLUDE=
DEBUG=	-O
CDEFS=	-DSINCE
CFLAGS= ${DEBUG} ${CDEFS} ${INCLUDE}

HDR=	
SRC=	stat.c
OBJ=	stat.o
SOURCE=	Makefile ${HDR} ${SRC}

all: stat

stat:
	${CC} -o $@ ${CFLAGS} stat.c

clean: FRC
	rm -f Makefile.bak stat *.o a.out core errs tags

depend: ${SRC} ${HDR} FRC
	maketd -a ${CDEFS} ${INCLUDE} -b stat.c

install: all FRC
	install -c -s -m 751 -o binary -g system stat ${BIN}

lint: ${HDR} ${SRC} FRC
	lint -hnx ${CDEFS} ${INCLUDE} ${SRC}

print: source FRC
	lpr -J'stat source' ${SOURCE}

source: ${SOURCE}

spotless: clean
	rcsclean ${SOURCE}

tags: ${SRC} ${HDR}
	ctags -t ${SRC} ${HDR}

${SOURCE}:
	co $@

FRC:

# DO NOT DELETE THIS LINE - make depend DEPENDS ON IT

stat: $I/ctype.h $I/grp.h $I/pwd.h $I/stdio.h $S/stat.h $S/time.h $S/types.h \
	$I/time.h stat.c

# *** Do not add anything here - It will go away. ***
