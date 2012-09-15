/*
 * Errors and exceptions.
 *
 * Copyright (C) 1989 by Kenneth Almquist.  All rights reserved.
 * This file is part of ash, which is distributed under the terms specified
 * by the Ash General Public License.  See the file named LICENSE.
 */

#include "shell.h"
#include "main.h"
#include "options.h"
#include "output.h"
#include "error.h"
#include <signal.h>
#ifdef __STDC__
#include "stdarg.h"
#else
#include <varargs.h>	
#endif
#include "myerrno.h"


/*
 * Code to handle exceptions in C.
 */

struct jmploc *handler;
int exception;
volatile int suppressint;
volatile int intpending;
char *commandname;


/*
 * Called to raise an exception.  Since C doesn't include exceptions, we
 * just do a longjmp to the exception handler.  The type of exception is
 * stored in the global variable "exception".
 */

void
raise(e) {
      if (handler == NULL)
	    abort();
      exception = e;
      longjmp(handler->loc, 1);
}


/*
 * Called from trap.c when a SIGINT is received.  (If the user specifies
 * that SIGINT is to be trapped or ignored using the trap builtin, then
 * this routine is not called.)  Suppressint is nonzero when interrupts
 * are held using the INTOFF macro.  The call to _exit is necessary because
 * there is a short period after a fork before the signal handlers are
 * set to the appropriate value for the child.  (The test for iflag is
 * just defensive programming.)
 */

void
onint() {
      if (suppressint) {
	    intpending++;
	    return;
      }
      intpending = 0;
#ifdef BSD
      sigsetmask(0);
#endif
      if (rootshell && iflag)
	    raise(EXINT);
      else
	    _exit(128 + SIGINT);
}



void
error2(a, b)
      char *a, *b;
      {
      error("%s: %s", a, b);
}


/*
 * Error is called to raise the error exception.  If the first argument
 * is not NULL then error prints an error message using printf style
 * formatting.  It then raises the error exception.
 */

#ifdef __STDC__
void
error(char *msg, ...) {
#else
void
error(va_alist)
      va_dcl
      {
      char *msg;
#endif
      va_list ap;

      CLEAR_PENDING_INT;
      INTOFF;
#ifdef __STDC__
      va_start(ap, msg);
#else
      va_start(ap);
      msg = va_arg(ap, char *);
#endif
#ifdef DEBUG
      if (msg)
	    TRACE(("error(\"%s\") pid=%d\n", msg, getpid()));
      else
	    TRACE(("error(NULL) pid=%d\n", getpid()));
#endif
      if (msg) {
	    if (commandname)
		  outfmt(&errout, "%s: ", commandname);
	    doformat(&errout, msg, ap);
	    out2c('\n');
      }
      va_end(ap);
      flushall();
      raise(EXERROR);
}



/*
 * Table of error messages.
 */

struct errname {
      short errcode;		/* error number */
      short action;		/* operation which encountered the error */
      char *msg;		/* text describing the error */
};


#define ALL (E_OPEN|E_CREAT|E_EXEC)

STATIC const struct errname errormsg[] = {
      EINTR, ALL,	"interrupted",
      EACCES, ALL,	"permission denied",
      EIO, ALL,		"I/O error",
      ENOENT, E_OPEN,	"no such file",
      ENOENT, E_CREAT,	"directory nonexistent",
      ENOENT, E_EXEC,	"not found",
      ENOTDIR, E_OPEN,	"no such file",
      ENOTDIR, E_CREAT,	"directory nonexistent",
      ENOTDIR, E_EXEC,	"not found",
      EISDIR, ALL,	"is a directory",
/*    EMFILE, ALL,	"too many open files", */
      ENFILE, ALL,	"file table overflow",
      ENOSPC, ALL,	"file system full",
#ifdef EDQUOT
      EDQUOT, ALL,	"disk quota exceeded",
#endif
#ifdef ENOSR
      ENOSR, ALL,	"no streams resources",
#endif
      ENXIO, ALL,	"no such device or address",
      EROFS, ALL,	"read-only file system",
      ETXTBSY, ALL,	"text busy",
#ifdef SYSV
      EAGAIN, E_EXEC,	"not enough memory",
#endif
      ENOMEM, ALL,	"not enough memory",
#ifdef ENOLINK
      ENOLINK, ALL,	"remote access failed"
#endif
#ifdef EMULTIHOP
      EMULTIHOP, ALL,	"remote access failed",
#endif
#ifdef ECOMM
      ECOMM, ALL,	"remote access failed",
#endif
#ifdef ESTALE
      ESTALE, ALL,	"remote access failed",
#endif
#ifdef ETIMEDOUT
      ETIMEDOUT, ALL,	"remote access failed",
#endif
#ifdef ELOOP
      ELOOP, ALL,	"symbolic link loop",
#endif
      E2BIG, E_EXEC,	"argument list too long",
#ifdef ELIBACC
      ELIBACC, E_EXEC,	"shared library missing",
#endif
      0, 0,		NULL
};


/*
 * Return a string describing an error.  The returned string may be a
 * pointer to a static buffer that will be overwritten on the next call.
 * Action describes the operation that got the error.
 */

char *
errmsg(e, action) {
      struct errname const *ep;
      static char buf[12];

      for (ep = errormsg ; ep->errcode ; ep++) {
	    if (ep->errcode == e && (ep->action & action) != 0)
		  return ep->msg;
      }
      fmtstr(buf, sizeof buf, "error %d", e);
      return buf;
}
