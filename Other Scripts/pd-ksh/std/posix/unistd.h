/* unistd.h: misc. P1003.1 definitions */
/* Based on a version by Terrence W. Holm */

#if ! _UNISTD_H
#define	_UNISTD_H 1

#include <stddef.h>

/* doesn't really belong here, but the library function need it */
/* todo: use _ARGS, _Void, _Const */
#if __STDC__
#define	ARGS(args)	args
#define Void	void
#define	Const	const
#else
#define	ARGS(args)	()
#define	Void	char
#define	Const
#endif

#include <io.h>			/* POSIX IO functions */

/*  for access(2)  */

#define	R_OK	4
#define	W_OK	2
#define	X_OK	1
#define	F_OK	0

/*  for lockf(2)  */

#define	F_ULOCK	0
#define	F_LOCK	1
#define	F_TLOCK	2
#define	F_TEST	3

/*  for lseek(2)  */

#define	SEEK_SET	0
#define	SEEK_CUR	1
#define	SEEK_END	2

#define	IN_PATH	"/usr/include"

char   *getcwd ARGS ((char *buf, size_t len));

#endif

