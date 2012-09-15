/*
 * Copyright (C) 1989 by Kenneth Almquist.  All rights reserved.
 * This file is part of ash, which is distributed under the terms specified
 * by the Ash General Public License.  See the file named LICENSE.
 */

#include <stdio.h>
#include "shell.h"
#include "parser.h"
#include "nodes.h"
#include "mystring.h"


#ifdef notdef
static shtree(), shcmd(), sharg(), indent();


showtree(n)
      union node *n;
      {
      trputs("showtree called\n");
      shtree(n, 1, NULL, stdout);
}


static
shtree(n, ind, pfx, fp)
      union node *n;
      char *pfx;
      FILE *fp;
      {
      struct nodelist *lp;
      char *s;

      indent(ind, pfx, fp);
      switch(n->type) {
      case NSEMI:
	    s = "; ";
	    goto binop;
      case NAND:
	    s = " && ";
	    goto binop;
      case NOR:
	    s = " || ";
binop:
	    shtree(n->nbinary.ch1, ind, NULL, fp);
	    if (ind < 0)
		  fputs(s, fp);
	    shtree(n->nbinary.ch2, ind, NULL, fp);
	    break;
      case NCMD:
	    shcmd(n, fp);
	    if (ind >= 0)
		  putc('\n', fp);
	    break;
      case NPIPE:
	    for (lp = n->npipe.cmdlist ; lp ; lp = lp->next) {
		  shcmd(lp->n, fp);
		  if (lp->next)
			fputs(" | ", fp);
	    }
	    if (n->npipe.backgnd)
		  fputs(" &", fp);
	    if (ind >= 0)
		  putc('\n', fp);
	    break;
      default:
	    fprintf(fp, "<node type %d>", n->type);
	    if (ind >= 0)
		  putc('\n', fp);
	    break;
      }
}



static
shcmd(cmd, fp)
      union node *cmd;
      FILE *fp;
      {
      union node *np;
      int first;
      char *s;
      int dftfd;

      first = 1;
      for (np = cmd->ncmd.args ; np ; np = np->narg.next) {
	    if (! first)
		  putchar(' ');
	    sharg(np, fp);
	    first = 0;
      }
      for (np = cmd->ncmd.redirect ; np ; np = np->nfile.next) {
	    if (! first)
		  putchar(' ');
	    switch (np->nfile.type) {
		  case NTO:	s = ">";  dftfd = 1; break;
		  case NAPPEND:	s = ">>"; dftfd = 1; break;
		  case NTOFD:	s = ">&"; dftfd = 1; break;
		  case NFROM:	s = "<";  dftfd = 0; break;
		  case NFROMFD:	s = "<&"; dftfd = 0; break;
	    }
	    if (np->nfile.fd != dftfd)
		  fprintf(fp, "%d", np->nfile.fd);
	    fputs(s, fp);
	    if (np->nfile.type == NTOFD || np->nfile.type == NFROMFD) {
		  fprintf(fp, "%d", np->nfile.dupfd);
	    } else {
		  sharg(np->nfile.fname, fp);
	    }
	    first = 0;
      }
}



static
sharg(arg, fp)
      union node *arg;
      FILE *fp;
      {
      char *p;
      struct nodelist *bqlist;

      if (arg->type != NARG) {
	    printf("<node type %d>\n", arg->type);
	    fflush(stdout);
	    abort();
      }
      bqlist = arg->narg.backquote;
      for (p = arg->narg.text ; *p ; p++) {
	    switch (*p) {
	    case CTLESC:
		  putc(*++p, fp);
		  break;
	    case CTLVAR:
	    case CTLVAR|CTLQUOTE:
		  putc('$', fp);
		  break;
	    case CTLBACKQ:
	    case CTLBACKQ|CTLQUOTE:
		  putc('`', fp);
		  shtree(bqlist->n, -1, NULL, fp);
		  putc('`', fp);
		  break;
	    default:
		  putc(*p, fp);
		  break;
	    }
      }
}


static
indent(amount, pfx, fp)
      char *pfx;
      FILE *fp;
      {
      int i;

      for (i = 0 ; i < amount ; i++) {
	    if (pfx && i == amount - 1)
		  fputs(pfx, fp);
	    putc('\t', fp);
      }
}
#endif



/*
 * Debugging stuff.
 */


FILE *tracefile;



trputc(c) {
#ifdef DEBUG
      if (tracefile == NULL)
	    return;
      putc(c, tracefile);
      if (c == '\n')
	    fflush(tracefile);
#endif
}


trace(fmt, a1, a2, a3, a4, a5, a6, a7, a8)
      char *fmt;
      {
#ifdef DEBUG
      if (tracefile == NULL)
	    return;
      fprintf(tracefile, fmt, a1, a2, a3, a4, a5, a6, a7, a8);
      if (strchr(fmt, '\n'))
	    fflush(tracefile);
#endif
}


trputs(s)
      char *s;
      {
#ifdef DEBUG
      if (tracefile == NULL)
	    return;
      fputs(s, tracefile);
      if (strchr(s, '\n'))
	    fflush(tracefile);
#endif
}


trstring(s)
      char *s;
      {
      register char *p;
      char c;

#ifdef DEBUG
      if (tracefile == NULL)
	    return;
      putc('"', tracefile);
      for (p = s ; *p ; p++) {
	    switch (*p) {
	    case '\n':  c = 'n';  goto backslash;
	    case '\t':  c = 't';  goto backslash;
	    case '\r':  c = 'r';  goto backslash;
	    case '"':  c = '"';  goto backslash;
	    case '\\':  c = '\\';  goto backslash;
	    case CTLESC:  c = 'e';  goto backslash;
	    case CTLVAR:  c = 'v';  goto backslash;
	    case CTLVAR+CTLQUOTE:  c = 'V';  goto backslash;
	    case CTLBACKQ:  c = 'q';  goto backslash;
	    case CTLBACKQ+CTLQUOTE:  c = 'Q';  goto backslash;
backslash:	  putc('\\', tracefile);
		  putc(c, tracefile);
		  break;
	    default:
		  if (*p >= ' ' && *p <= '~')
			putc(*p, tracefile);
		  else {
			putc('\\', tracefile);
			putc(*p >> 6 & 03, tracefile);
			putc(*p >> 3 & 07, tracefile);
			putc(*p & 07, tracefile);
		  }
		  break;
	    }
      }
      putc('"', tracefile);
#endif
}


trargs(ap)
      char **ap;
      {
#ifdef DEBUG
      if (tracefile == NULL)
	    return;
      while (*ap) {
	    trstring(*ap++);
	    if (*ap)
		  putc(' ', tracefile);
	    else
		  putc('\n', tracefile);
      }
      fflush(tracefile);
#endif
}


opentrace() {
      char s[100];
      char *p;
      char *getenv();
      int flags;

#ifdef DEBUG
      if ((p = getenv("HOME")) == NULL)
	    p = "/tmp";
      scopy(p, s);
      strcat(s, "/trace");
      if ((tracefile = fopen(s, "a")) == NULL) {
	    fprintf(stderr, "Can't open %s\n", s);
	    exit(2);
      }
#ifdef O_APPEND
      if ((flags = fcntl(fileno(tracefile), F_GETFL, 0)) >= 0)
	    fcntl(fileno(tracefile), F_SETFL, flags | O_APPEND);
#endif
      fputs("\nTracing started.\n", tracefile);
      fflush(tracefile);
#endif
}
