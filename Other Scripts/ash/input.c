/*
 * Copyright (C) 1989 by Kenneth Almquist.  All rights reserved.
 * This file is part of ash, which is distributed under the terms specified
 * by the Ash General Public License.  See the file named LICENSE.
 *
 * This file implements the input routines used by the parser.
 */

#include <stdio.h>	/* defines BUFSIZ */
#include "shell.h"
#include "syntax.h"
#include "input.h"
#include "output.h"
#include "memalloc.h"
#include "error.h"
#include <fcntl.h>
#include "myerrno.h"

#define EOF_NLEFT -99		/* value of parsenleft when EOF pushed back */


/*
 * The parsefile structure pointed to by the global variable parsefile
 * contains information about the current file being read.
 */

MKINIT
struct parsefile {
      int linno;		/* current line */
      int fd;			/* file descriptor (or -1 if string) */
      int nleft;		/* number of chars left in buffer */
      char *nextc;		/* next char in buffer */
      struct parsefile *prev;	/* preceding file on stack */
      char *buf;		/* input buffer */
};


int plinno = 1;			/* input line number */
MKINIT int parsenleft;		/* copy of parsefile->nleft */
char *parsenextc;		/* copy of parsefile->nextc */
MKINIT struct parsefile basepf;	/* top level input file */
char basebuf[BUFSIZ];		/* buffer for top level input file */
struct parsefile *parsefile = &basepf;	/* current input file */
char *pushedstring;		/* copy of parsenextc when text pushed back */
int pushednleft;		/* copy of parsenleft when text pushed back */

#ifdef __STDC__
STATIC void pushfile(void);
#else
STATIC void pushfile();
#endif



#ifdef mkinit
INCLUDE "input.h"
INCLUDE "error.h"

INIT {
      extern char basebuf[];

      basepf.nextc = basepf.buf = basebuf;
}

RESET {
      if (exception != EXSHELLPROC)
	    parsenleft = 0;            /* clear input buffer */
      popallfiles();
}

SHELLPROC {
      popallfiles();
}
#endif


/*
 * Read a line from the script.
 */

char *
pfgets(line, len)
      char *line;
      {
      register char *p = line;
      int nleft = len;
      int c;

      while (--nleft > 0) {
	    c = pgetc_macro();
	    if (c == PEOF) {
		  if (p == line)
			return NULL;
		  break;
	    }
	    *p++ = c;
	    if (c == '\n')
		  break;
      }
      *p = '\0';
      return line;
}



/*
 * Read a character from the script, returning PEOF on end of file.
 * Nul characters in the input are silently discarded.
 */

int
pgetc() {
      return pgetc_macro();
}


/*
 * Refill the input buffer and return the next input character:
 *
 * 1) If a string was pushed back on the input, switch back to the regular
 *    buffer.
 * 2) If an EOF was pushed back (parsenleft == EOF_NLEFT) or we are reading
 *    from a string so we can't refill the buffer, return EOF.
 * 3) Call read to read in the characters.
 * 4) Delete all nul characters from the buffer.
 */

int
preadbuffer() {
      register char *p, *q;
      register int i;

      if (pushedstring) {
	    parsenextc = pushedstring;
	    pushedstring = NULL;
	    parsenleft = pushednleft;
	    if (--parsenleft >= 0)
		  return *parsenextc++;
      }
      if (parsenleft == EOF_NLEFT || parsefile->buf == NULL)
	    return PEOF;
      flushout(&output);
      flushout(&errout);
retry:
      p = parsenextc = parsefile->buf;
      i = read(parsefile->fd, p, BUFSIZ);
      if (i <= 0) {
	    if (i < 0) {
		  if (errno == EINTR)
			goto retry;
#ifdef BSD
		  if (parsefile->fd == 0 && errno == EWOULDBLOCK) {
			int flags = fcntl(0, F_GETFL, 0);
			if (flags >= 0 && flags & FNDELAY) {
			      flags &=~ FNDELAY;
			      if (fcntl(0, F_SETFL, flags) >= 0) {
				    out2str("ash: turning off NDELAY mode\n");
				    goto retry;
			      }
			}
		  }
#endif
	    }
#ifdef SYSV
	    if (i == 0 && parsefile->fd == 0) {
		  int flags = fcntl(0, F_GETFL, 0);
		  if (flags >= 0 && flags & O_NDELAY) {
			flags &=~ O_NDELAY;
			if (fcntl(0, F_SETFL, flags) >= 0) {
			      out2str("ash: turning off NDELAY mode\n");
			      goto retry;
			}
		  }
	    }
#endif
	    parsenleft = EOF_NLEFT;
	    return PEOF;
      }
      parsenleft = i - 1;

      /* delete nul characters */
      for (;;) {
	    if (*p++ == '\0')
		  break;
	    if (--i <= 0)
		  return *parsenextc++;		/* no nul characters */
      }
      q = p - 1;
      while (--i > 0) {
	    if (*p != '\0')
		  *q++ = *p;
	    p++;
      }
      if (q == parsefile->buf)
	    goto retry;			/* buffer contained nothing but nuls */
      parsenleft = q - parsefile->buf - 1;
      return *parsenextc++;
}


/*
 * Undo the last call to pgetc.  Only one character may be pushed back.
 * PEOF may be pushed back.
 */

void
pungetc() {
      parsenleft++;
      parsenextc--;
}


/*
 * Push a string back onto the input.  This code doesn't work if the user
 * tries to push back more than one string at once.
 */

void
ppushback(string, length)
      char *string;
      {
      pushedstring = parsenextc;
      pushednleft = parsenleft;
      parsenextc = string;
      parsenleft = length;
}



/*
 * Set the input to take input from a file.  If push is set, push the
 * old input onto the stack first.
 */

void
setinputfile(fname, push)
      char *fname;
      {
      int fd;
      int fd2;

      INTOFF;
      if ((fd = open(fname, O_RDONLY)) < 0)
	    error("Can't open %s", fname);
      if (fd < 10) {
	    fd2 = copyfd(fd, 10);
	    close(fd);
	    if (fd2 < 0)
		  error("Out of file descriptors");
	    fd = fd2;
      }
      setinputfd(fd, push);
      INTON;
}


/*
 * Like setinputfile, but takes an open file descriptor.  Call this with
 * interrupts off.
 */

void
setinputfd(fd, push) {
      if (push) {
	    pushfile();
	    parsefile->buf = ckmalloc(BUFSIZ);
      }
      if (parsefile->fd > 0)
	    close(parsefile->fd);
      parsefile->fd = fd;
      if (parsefile->buf == NULL)
	    parsefile->buf = ckmalloc(BUFSIZ);
      parsenleft = 0;
      plinno = 1;
}


/*
 * Like setinputfile, but takes input from a string.
 */

void
setinputstring(string, push)
      char *string;
      {
      INTOFF;
      if (push)
	    pushfile();
      parsenextc = string;
      parsenleft = strlen(string);
      parsefile->buf = NULL;
      plinno = 1;
      INTON;
}



/*
 * To handle the "." command, a stack of input files is used.  Pushfile
 * adds a new entry to the stack and popfile restores the previous level.
 */

STATIC void
pushfile() {
      struct parsefile *pf;

      parsefile->nleft = parsenleft;
      parsefile->nextc = parsenextc;
      parsefile->linno = plinno;
      pf = (struct parsefile *)ckmalloc(sizeof (struct parsefile));
      pf->prev = parsefile;
      pf->fd = -1;
      parsefile = pf;
}


void
popfile() {
      struct parsefile *pf = parsefile;

      INTOFF;
      if (pf->fd >= 0)
	    close(pf->fd);
      if (pf->buf)
	    ckfree(pf->buf);
      parsefile = pf->prev;
      ckfree(pf);
      parsenleft = parsefile->nleft;
      parsenextc = parsefile->nextc;
      plinno = parsefile->linno;
      INTON;
}


/*
 * Return to top level.
 */

void
popallfiles() {
      while (parsefile != &basepf)
	    popfile();
}



/*
 * Close the file(s) that the shell is reading commands from.  Called
 * after a fork is done.
 */

void
closescript() {
      popallfiles();
      if (parsefile->fd > 0) {
	    close(parsefile->fd);
	    parsefile->fd = 0;
      }
}
