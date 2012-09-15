/*
 * sprintf and vsprintf
 */

/* $Header */

#if __STDC__
#include <stdarg.h>
#else
#include <varargs.h>
#endif
#include <stdio.h>

#if _V7 || _BSD

int
#if __STDC__
sprintf(char *s, Const char *fmt, ...) {
#else
sprintf(va_alist) va_dcl
{
	char *s;
	char *fmt;
#endif
	register va_list va;
	int n;

#if __STDC__
	va_start(va, fmt);
#else
	va_start(va);
	s = va_arg(va, char *);
	fmt = va_arg(va, char *);
#endif
	n = vsprintf(s, fmt, va);
	va_end(va);
	return n;
}

int
#if __STDC__
vsprintf(char *s, const char *fmt, va_list va) {
#else
vsprintf(s, fmt, va)
	char *s;
	char *fmt;
	va_list va;
{
#endif
	int n;
	static FILE siob;

	siob._flag = _IOWRT;
	siob._base = siob._ptr = s;
	siob._cnt = BUFSIZ;
	siob._file = -1;

	n = vfprintf(&siob, fmt, va);
	*siob._ptr = 0;
	return n;
}

#endif

