#
# Makefile for Pcal v4.2 under UNIX
#

# Set the configuration variables below to taste.

CC	= /bin/cc
MANDIR	= /usr/man
COPTS	=
OBJECTS = pcal.o exprpars.o moonphas.o pcalutil.o readfile.o writefil.o

pcal:	$(OBJECTS)
	$(CC) $(LDFLAGS) -o pcal $(OBJECTS) -lm
	@ echo "Build of pcal complete"

exprpars.o:	exprpars.c pcaldefs.h
	$(CC) $(CFLAGS) $(COPTS) -c exprpars.c

moonphas.o:	moonphas.c pcaldefs.h pcalglob.h
	$(CC) $(CFLAGS) $(COPTS) -c moonphas.c

pcal.o:		pcal.c pcaldefs.h pcalglob.h pcallang.h
	$(CC) $(CFLAGS) $(COPTS) -c pcal.c

pcalutil.o:	pcalutil.c pcaldefs.h pcalglob.h pcallang.h
	$(CC) $(CFLAGS) $(COPTS) -c pcalutil.c

readfile.o:	readfile.c pcaldefs.h pcalglob.h pcallang.h
	$(CC) $(CFLAGS) $(COPTS) -c readfile.c

writefil.o:	writefil.c pcaldefs.h pcalglob.h pcallang.h pcalinit.h
	$(CC) $(CFLAGS) $(COPTS) -c writefil.c

pcalinit: pcalinit.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(COPTS) -o pcalinit pcalinit.c

pcalinit.h: pcalinit pcalinit.ps
	pcalinit pcalinit.ps pcalinit.h

pcalglob.h: protos.h noprotos.h

clean:
	rm -f $(OBJECTS) pcal pcalinit pcalinit.h

man:	pcal.man
	nroff -man pcal.man > pcal.1
	pack pcal.1
#	mv pcal.1.z $(MANDIR)
