/*
 * ANSI stream IO
 *
 * Heavily dependent on /usr/include/stdio.h being AT&T derived.
 * todo: needs L_* constants.
 */

/* $Header: stdio.h,v 1.2 88/06/23 18:32:35 egisin Locked $ */

#if ! _STDIO_H
#define	_STDIO_H 1

#include <stddef.h>

/* system stdio.h goes here ... %%% */
/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)stdio.h	5.3 (Berkeley) 3/15/86
 */

# ifndef FILE
#define	BUFSIZ	1024
extern	struct	_iobuf {
	int	_cnt;
	char	*_ptr;		/* should be unsigned char */
	char	*_base;		/* ditto */
	int	_bufsiz;
	short	_flag;
	char	_file;		/* should be short */
} _iob[];

#define	_IOREAD	01
#define	_IOWRT	02
#define	_IONBF	04
#define	_IOMYBUF	010
#define	_IOEOF	020
#define	_IOERR	040
#define	_IOSTRG	0100
#define	_IOLBF	0200
#define	_IORW	0400
#define	FILE	struct _iobuf
#define	EOF	(-1)

#define	stdin	(&_iob[0])
#define	stdout	(&_iob[1])
#define	stderr	(&_iob[2])
#ifndef lint
#define	getc(p)		(--(p)->_cnt>=0? (int)(*(unsigned char *)(p)->_ptr++):_filbuf(p))
#endif /*not lint*/
#define	getchar()	getc(stdin)
#ifndef lint
#define putc(x, p)	(--(p)->_cnt >= 0 ?\
	(int)(*(unsigned char *)(p)->_ptr++ = (x)) :\
	(((p)->_flag & _IOLBF) && -(p)->_cnt < (p)->_bufsiz ?\
		((*(p)->_ptr = (x)) != '\n' ?\
			(int)(*(unsigned char *)(p)->_ptr++) :\
			_flsbuf(*(unsigned char *)(p)->_ptr, p)) :\
		_flsbuf((unsigned char)(x), p)))
#endif /*not lint*/
#define	putchar(x)	putc(x,stdout)
#define	feof(p)		(((p)->_flag&_IOEOF)!=0)
#define	ferror(p)	(((p)->_flag&_IOERR)!=0)
#define	fileno(p)	((p)->_file)
#define	clearerr(p)	((p)->_flag &= ~(_IOERR|_IOEOF))


#ifdef lint
extern int printf(/*FORMAT1*/);
extern int fprintf(/*FORMAT2*/);
#endif /*lint*/

FILE	*fopen();
FILE	*fdopen();
FILE	*freopen();
FILE	*popen();
long	ftell();
char	*fgets();
char	*gets();
#ifdef vax
#endif
# endif
/* ... end system stdio.h */
#line 15 "stdio.h"

#ifndef _IOFBF
#define	_IOFBF	00000
#endif
#ifndef _IOLBF
#define	_IOLBF	00000		/* same as _IOFBF */
#endif

#define	SEEK_SET	0
#define	SEEK_CUR	1
#define	SEEK_END	2

#if _SYSV
#define	_EXTERN	extern
#else
#define	_EXTERN	
#endif

_EXTERN	int	remove ARGS((const char *name));
_EXTERN	int	rename ARGS((const char *oname, const char *nname));
_EXTERN	FILE   *tmpfile ARGS((void));
_EXTERN	char   *tmpnam ARGS((char *s));

_EXTERN	FILE   *fopen ARGS((const char *name, const char *mode));
_EXTERN	FILE   *freopen ARGS((const char *name, const char *mode, FILE *f));
_EXTERN	FILE   *fdopen ARGS((int fd, const char *mode));
_EXTERN	int	fflush ARGS((FILE *f));
_EXTERN	int	fclose ARGS((FILE *f));
_EXTERN	void	setbuf ARGS((FILE *f, char *buf));
_EXTERN	int	setvbuf ARGS((FILE *f, char *buf, int flags, size_t len));

_EXTERN	int	fseek ARGS((FILE *f, long off, int how));
_EXTERN	long	ftell ARGS((FILE *f));
_EXTERN	void	rewind ARGS((FILE *f));

_EXTERN	int	printf ARGS((const char *fmt, ...));
_EXTERN	int	fprintf ARGS((FILE *f, const char *fmt, ...));
_EXTERN	int	sprintf ARGS((char *s, const char *fmt, ...));
/* we do not include <stdarg.h> to prevent <varargs.h> conflicts */
_EXTERN	int	vprintf ARGS((const char *fmt, Void *va));
_EXTERN	int	vfprintf ARGS((FILE *f, const char *fmt, Void *va));
_EXTERN	int	vsprintf ARGS((char *s, const char *fmt, Void *va));
_EXTERN	int	scanf ARGS((const char *fmt, ...));
_EXTERN	int	fscanf ARGS((FILE *f, const char *fmt, ...));
_EXTERN	int	sscanf ARGS((const char *s, const char *fmt, ...));

_EXTERN	size_t	fread ARGS((void *ptr, size_t size, size_t n, FILE *f));
_EXTERN	size_t	frwrite ARGS((const void *ptr, size_t size, size_t n, FILE *f));
_EXTERN	int	fgetc ARGS((FILE *f));
_EXTERN	int	fputc ARGS((int c, FILE *f));
_EXTERN	char   *fgets ARGS((char *s, int len, FILE *f));
_EXTERN	int	fputs ARGS((const char *s, FILE *f));
_EXTERN	char   *gets ARGS((char *s));
_EXTERN	int     puts ARGS((const char *s));

#endif

