/*
 * Copyright 1987, 1988 by MIT Student Information Processing Board.
 *
 * For copyright info, see mit-sipb-copyright.h.
 */

#include <stdio.h>
#include "mit-sipb-copyright.h"

/*
 * Our environment only provides for ANSI's <stdarg.h> when using GNU
 * C.  Grump grump...
 */
#if ! __GNUC__
#define VARARGS 1
#endif

/* We don't have the v*printf routines... */
#define vfprintf(stream,fmt,args) _doprnt(fmt,args,stream)

#if __STDC__ && !VARARGS
#	include <stdarg.h>
#else /* varargs: not STDC or no <stdarg> */
	/* Non-ANSI, always take <varargs.h> path. */
#	undef VARARGS
#	define VARARGS 1
#	include <varargs.h>
#	undef vfprintf
#	define vfprintf(stream,fmt,args) _doprnt(fmt,args,stream)
#endif /* varargs */

#include "error_table.h"
#include "internal.h"

/*
 * Protect us from header version (externally visible) of com_err, so
 * we can survive in a <varargs.h> environment.  I think.
 */
#define com_err com_err_external
#include "com_err.h"
#undef com_err

/* BSD. sigh. */
#undef vfprintf
#define vfprintf(stream,fmt,args) _doprnt(fmt,args,stream)

#if ! lint
static const char rcsid[] =
    "$Header: /afs/rel-eng.athena.mit.edu/project/release/current/source/athena/athena.lib/et/RCS/com_err.c,v 1.2 90/03/23 13:22:20 epeisach Exp $";
#endif	/* ! lint */

static void
#ifdef __STDC__
    default_com_err_proc (const char *whoami, long code, const char *fmt, va_list args)
#else
    default_com_err_proc (whoami, code, fmt, args)
    const char *whoami;
    long code;
    const char *fmt;
    va_list args;
#endif
{
    if (whoami) {
	fputs(whoami, stderr);
	fputs(": ", stderr);
    }
    if (code) {
	fputs(error_message(code), stderr);
	fputs(" ", stderr);
    }
    if (fmt) {
        vfprintf (stderr, fmt, args);
    }
    putc('\n', stderr);
    /* should do this only on a tty in raw mode */
    putc('\r', stderr);
    fflush(stderr);
}

#ifdef __STDC__
typedef void (*errf) (const char *, long, const char *, va_list);
#else
typedef void (*errf) ();
#endif

errf com_err_hook = default_com_err_proc;

void com_err_va (whoami, code, fmt, args)
    const char *whoami;
    long code;
    const char *fmt;
    va_list args;
{
    (*com_err_hook) (whoami, code, fmt, args);
}

#if ! VARARGS
void com_err (const char *whoami,
	      long code,
	      const char *fmt, ...)
{
#else
void com_err (va_alist)
    va_dcl
{
    const char *whoami, *fmt;
    long code;
#endif
    va_list pvar;

    if (!com_err_hook)
	com_err_hook = default_com_err_proc;
#if VARARGS
    va_start (pvar);
    whoami = va_arg (pvar, const char *);
    code = va_arg (pvar, long);
    fmt = va_arg (pvar, const char *);
#else
    va_start(pvar, fmt);
#endif
    com_err_va (whoami, code, fmt, pvar);
    va_end(pvar);
}

errf set_com_err_hook (new_proc)
    errf new_proc;
{
    errf x = com_err_hook;

    if (new_proc)
	com_err_hook = new_proc;
    else
	com_err_hook = default_com_err_proc;

    return x;
}

errf reset_com_err_hook () {
    errf x = com_err_hook;
    com_err_hook = default_com_err_proc;
    return x;
}
