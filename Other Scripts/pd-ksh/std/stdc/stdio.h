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
/*	@(#)stdio.h	2.7	*/
#ifndef _NFILE
#define _NFILE	50

#if u370
#define BUFSIZ	4096
#endif
#if m68k || vax || u3b || u3b5
#define BUFSIZ	1024
#endif
#if pdp11
#define BUFSIZ	512
#endif

/* buffer size for multi-character output to unbuffered files */
#define _SBFSIZ 8

typedef struct {
#if m68k || vax || u3b || u3b5
	int	_cnt;
	unsigned char	*_ptr;
#else
	unsigned char	*_ptr;
	int	_cnt;
#endif
	unsigned char	*_base;
	char	_flag;
	char	_file;
} FILE;

/*
 * _IOLBF means that a file's output will be buffered line by line
 * In addition to being flags, _IONBF, _IOLBF and _IOFBF are possible
 * values for "type" in setvbuf.
 */
#define _IOFBF		0000
#define _IOREAD		0001
#define _IOWRT		0002
#define _IONBF		0004
#define _IOMYBUF	0010
#define _IOEOF		0020
#define _IOERR		0040
#define _IOLBF		0100
#define _IORW		0200

#ifndef NULL
#endif
#ifndef EOF
#define EOF		(-1)
#endif

#define stdin		(&_iob[0])
#define stdout		(&_iob[1])
#define stderr		(&_iob[2])

#define _bufend(p)	_bufendtab[(p)->_file]
#define _bufsiz(p)	(_bufend(p) - (p)->_base)

#ifndef lint
#define getc(p)		(--(p)->_cnt < 0 ? _filbuf(p) : (int) *(p)->_ptr++)
#define putc(x, p)	(--(p)->_cnt < 0 ? \
			_flsbuf((unsigned char) (x), (p)) : \
			(int) (*(p)->_ptr++ = (unsigned char) (x)))
#define getchar()	getc(stdin)
#define putchar(x)	putc((x), stdout)
#define clearerr(p)	((void) ((p)->_flag &= ~(_IOERR | _IOEOF)))
#define feof(p)		((p)->_flag & _IOEOF)
#define ferror(p)	((p)->_flag & _IOERR)
#define fileno(p)	(p)->_file
#endif

extern FILE	_iob[_NFILE];
extern FILE	*fopen(), *fdopen(), *freopen(), *popen(), *tmpfile();
extern long	ftell();
extern void	rewind(), setbuf();
extern char	*ctermid(), *cuserid(), *fgets(), *gets(), *tempnam(), *tmpnam();
extern unsigned char *_bufendtab[];

#define L_ctermid	9
#define L_cuserid	9
#define P_tmpdir	"/usr/tmp/"
#define L_tmpnam	(sizeof(P_tmpdir) + 15)
#endif
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

