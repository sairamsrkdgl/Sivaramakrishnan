/*
 * Copyright (C) 1989 by Kenneth Almquist.  All rights reserved.
 * This file is part of ash, which is distributed under the terms specified
 * by the Ash General Public License.  See the file named LICENSE.
 */

#include "shell.h"
#include "parser.h"
#include "nodes.h"
#include "expand.h"	/* defines rmescapes() */
#include "redir.h"	/* defines copyfd() */
#include "syntax.h"
#include "options.h"
#include "input.h"
#include "output.h"
#include "var.h"
#include "error.h"
#include "memalloc.h"
#include "mystring.h"


/*
 * Shell command parser.
 */

#define EOFMARKLEN 79

/* values returned by readtoken */
#include "token.def"



struct heredoc {
      struct heredoc *next;	/* next here document in list */
      union node *here;		/* redirection node */
      char *eofmark;		/* string indicating end of input */
      int striptabs;		/* if set, strip leading tabs */
};



struct heredoc *heredoclist;	/* list of here documents to read */
int parsebackquote;		/* nonzero if we are inside backquotes */
int doprompt;			/* if set, prompt the user */
int needprompt;			/* true if interactive and at start of line */
int lasttoken;			/* last token read */
MKINIT int tokpushback;		/* last token pushed back */
char *wordtext;			/* text of last word returned by readtoken */
struct nodelist *backquotelist;
union node *redirnode;
struct heredoc *heredoc;
int quoteflag;			/* set if (part of) last token was quoted */
int startlinno;			/* line # where last token started */


#define GDB_HACK 1 /* avoid local declarations which gdb can't handle */
#ifdef GDB_HACK
static const char argvars[5] = {CTLVAR, VSNORMAL|VSQUOTE, '@', '=', '\0'};
static const char types[] = "}-+?=";
#endif


#ifdef __STDC__
STATIC union node *list(int);
STATIC union node *andor(void);
STATIC union node *pipeline(void);
STATIC union node *command(void);
STATIC union node *simplecmd(void);
STATIC void parsefname(void);
STATIC void parseheredoc(void);
STATIC void checkkwd(void);
STATIC int readtoken(void);
STATIC int readtoken1(int, char const *, char *, int);
STATIC void attyline(void);
STATIC int noexpand(char *);
STATIC void synexpect(int);
STATIC void synerror(char *);
#else
STATIC union node *list();
STATIC union node *andor();
STATIC union node *pipeline();
STATIC union node *command();
STATIC union node *simplecmd();
STATIC void parsefname();
STATIC void parseheredoc();
STATIC void checkkwd();
STATIC int readtoken();
STATIC int readtoken1();
STATIC void attyline();
STATIC int noexpand();
STATIC void synexpect();
STATIC void synerror();
#endif

#if ATTY
#ifdef __STDC__
STATIC void putprompt(char *);
#else
STATIC void putprompt();
#endif
#else /* not ATTY */
#define putprompt(s)	out2str(s)
#endif




/*
 * Read and parse a command.  Returns NEOF on end of file.  (NULL is a
 * valid parse tree indicating a blank line.)
 */

union node *
parsecmd(interact) {
      int t;

      doprompt = interact;
      if (doprompt)
	    putprompt(ps1val());
      needprompt = 0;
      if ((t = readtoken()) == TEOF)
	    return NEOF;
      if (t == TNL)
	    return NULL;
      tokpushback++;
      return list(1);
}


STATIC union node *
list(nlflag) {
      union node *n1, *n2, *n3;

      checkkwd();
      if (nlflag == 0 && tokendlist[lasttoken])
	    return NULL;
      n1 = andor();
      for (;;) {
	    switch (readtoken()) {
	    case TBACKGND:
		  if (n1->type == NCMD || n1->type == NPIPE) {
			n1->ncmd.backgnd = 1;
		  } else if (n1->type == NREDIR) {
			n1->type = NBACKGND;
		  } else {
			n3 = (union node *)stalloc(sizeof (struct nredir));
			n3->type = NBACKGND;
			n3->nredir.n = n1;
			n3->nredir.redirect = NULL;
			n1 = n3;
		  }
		  goto tsemi;
	    case TNL:
		  tokpushback++;
		  /* fall through */
tsemi:	    case TSEMI:
		  if (readtoken() == TNL) {
			parseheredoc();
			if (nlflag)
			      return n1;
		  } else {
			tokpushback++;
		  }
		  checkkwd();
		  if (tokendlist[lasttoken])
			return n1;
		  n2 = andor();
		  n3 = (union node *)stalloc(sizeof (struct nbinary));
		  n3->type = NSEMI;
		  n3->nbinary.ch1 = n1;
		  n3->nbinary.ch2 = n2;
		  n1 = n3;
		  break;
	    case TEOF:
		  if (heredoclist)
			parseheredoc();
		  else
			pungetc();		/* push back EOF on input */
		  return n1;
	    default:
		  if (nlflag)
			synexpect(-1);
		  tokpushback++;
		  return n1;
	    }
      }
}



STATIC union node *
andor() {
      union node *n1, *n2, *n3;
      int t;

      n1 = pipeline();
      for (;;) {
	    if ((t = readtoken()) == TAND) {
		  t = NAND;
	    } else if (t == TOR) {
		  t = NOR;
	    } else {
		  tokpushback++;
		  return n1;
	    }
	    n2 = pipeline();
	    n3 = (union node *)stalloc(sizeof (struct nbinary));
	    n3->type = t;
	    n3->nbinary.ch1 = n1;
	    n3->nbinary.ch2 = n2;
	    n1 = n3;
      }
}



STATIC union node *
pipeline() {
      union node *n1, *pipenode;
      struct nodelist *lp, *prev;

      n1 = command();
      if (readtoken() == TPIPE) {
	    pipenode = (union node *)stalloc(sizeof (struct npipe));
	    pipenode->type = NPIPE;
	    pipenode->npipe.backgnd = 0;
	    lp = (struct nodelist *)stalloc(sizeof (struct nodelist));
	    pipenode->npipe.cmdlist = lp;
	    lp->n = n1;
	    do {
		  prev = lp;
		  lp = (struct nodelist *)stalloc(sizeof (struct nodelist));
		  lp->n = command();
		  prev->next = lp;
	    } while (readtoken() == TPIPE);
	    lp->next = NULL;
	    n1 = pipenode;
      }
      tokpushback++;
      return n1;
}



STATIC union node *
command() {
      union node *n1, *n2;
      union node *ap, **app;
      union node *cp, **cpp;
      union node *redir, **rpp;
      int t;

      checkkwd();
      switch (readtoken()) {
      case TIF:
	    n1 = (union node *)stalloc(sizeof (struct nif));
	    n1->type = NIF;
	    n1->nif.test = list(0);
	    if (readtoken() != TTHEN)
		  synexpect(TTHEN);
	    n1->nif.ifpart = list(0);
	    n2 = n1;
	    while (readtoken() == TELIF) {
		  n2->nif.elsepart = (union node *)stalloc(sizeof (struct nif));
		  n2 = n2->nif.elsepart;
		  n2->type = NIF;
		  n2->nif.test = list(0);
		  if (readtoken() != TTHEN)
			synexpect(TTHEN);
		  n2->nif.ifpart = list(0);
	    }
	    if (lasttoken == TELSE)
		  n2->nif.elsepart = list(0);
	    else {
		  n2->nif.elsepart = NULL;
		  tokpushback++;
	    }
	    if (readtoken() != TFI)
		  synexpect(TFI);
	    break;
      case TWHILE:
      case TUNTIL:
	    n1 = (union node *)stalloc(sizeof (struct nbinary));
	    n1->type = (lasttoken == TWHILE)? NWHILE : NUNTIL;
	    n1->nbinary.ch1 = list(0);
	    if (readtoken() != TDO)
		  synexpect(TDO);
	    n1->nbinary.ch2 = list(0);
	    if (readtoken() != TDONE)
		  synexpect(TDONE);
	    break;
      case TFOR:
	    if (readtoken() != TWORD || quoteflag || ! goodname(wordtext))
		  synerror("Bad for loop variable");
	    n1 = (union node *)stalloc(sizeof (struct nfor));
	    n1->type = NFOR;
	    n1->nfor.var = wordtext;
	    if (readtoken() == TWORD && ! quoteflag && equal(wordtext, "in")) {
		  app = &ap;
		  while (readtoken() == TWORD) {
			n2 = (union node *)stalloc(sizeof (struct narg));
			n2->type = NARG;
			n2->narg.text = wordtext;
			n2->narg.backquote = backquotelist;
			*app = n2;
			app = &n2->narg.next;
		  }
		  *app = NULL;
		  n1->nfor.args = ap;
	    } else {
#ifndef GDB_HACK
		  static const char argvars[5] = {CTLVAR, VSNORMAL|VSQUOTE,
						  '@', '=', '\0'};
#endif
		  n2 = (union node *)stalloc(sizeof (struct narg));
		  n2->type = NARG;
		  n2->narg.text = (char *)argvars;
		  n2->narg.backquote = NULL;
		  n2->narg.next = NULL;
		  n1->nfor.args = n2;
	    }
	    if (lasttoken != TNL && lasttoken != TSEMI)
		  synexpect(-1);
	    checkkwd();
	    if ((t = readtoken()) == TDO)
		  t = TDONE;
	    else if (t == TBEGIN)
		  t = TEND;
	    else
		  synexpect(-1);
	    n1->nfor.body = list(0);
	    if (readtoken() != t)
		  synexpect(t);
	    break;
      case TCASE:
	    n1 = (union node *)stalloc(sizeof (struct ncase));
	    n1->type = NCASE;
	    if (readtoken() != TWORD)
		  synexpect(TWORD);
	    n1->ncase.expr = n2 = (union node *)stalloc(sizeof (struct narg));
	    n2->type = NARG;
	    n2->narg.text = wordtext;
	    n2->narg.backquote = backquotelist;
	    n2->narg.next = NULL;
	    while (readtoken() == TNL);
	    if (lasttoken != TWORD || ! equal(wordtext, "in"))
		  synerror("expecting \"in\"");
	    cpp = &n1->ncase.cases;
	    while (checkkwd(), readtoken() == TWORD) {
		  *cpp = cp = (union node *)stalloc(sizeof (struct nclist));
		  cp->type = NCLIST;
		  app = &cp->nclist.pattern;
		  for (;;) {
			*app = ap = (union node *)stalloc(sizeof (struct narg));
			ap->type = NARG;
			ap->narg.text = wordtext;
			ap->narg.backquote = backquotelist;
			if (readtoken() != TPIPE)
			      break;
			app = &ap->narg.next;
			if (readtoken() != TWORD)
			      synexpect(TWORD);
		  }
		  ap->narg.next = NULL;
		  if (lasttoken != TRP)
			synexpect(TRP);
		  cp->nclist.body = list(0);
		  if ((t = readtoken()) == TESAC)
			tokpushback++;
		  else if (t != TENDCASE)
			synexpect(TENDCASE);
		  cpp = &cp->nclist.next;
	    }
	    *cpp = NULL;
	    if (lasttoken != TESAC)
		  synexpect(TESAC);
	    break;
      case TLP:
	    n1 = (union node *)stalloc(sizeof (struct nredir));
	    n1->type = NSUBSHELL;
	    n1->nredir.n = list(0);
	    n1->nredir.redirect = NULL;
	    if (readtoken() != TRP)
		  synexpect(TRP);
	    break;
      case TBEGIN:
	    n1 = list(0);
	    if (readtoken() != TEND)
		  synexpect(TEND);
	    break;
      case TWORD:
      case TREDIR:
	    tokpushback++;
	    return simplecmd();
      default:
	    synexpect(-1);
      }

      /* Now check for redirection which may follow command */
      rpp = &redir;
      while (readtoken() == TREDIR) {
	    *rpp = n2 = redirnode;
	    rpp = &n2->nfile.next;
	    parsefname();
      }
      tokpushback++;
      *rpp = NULL;
      if (redir) {
	    if (n1->type != NSUBSHELL) {
		  n2 = (union node *)stalloc(sizeof (struct nredir));
		  n2->type = NREDIR;
		  n2->nredir.n = n1;
		  n1 = n2;
	    }
	    n1->nredir.redirect = redir;
      }
      return n1;
}


STATIC union node *
simplecmd() {
      union node *args, **app;
      union node *redir, **rpp;
      union node *n;

      args = NULL;
      app = &args;
      rpp = &redir;
      for (;;) {
	    if (readtoken() == TWORD) {
		  n = (union node *)stalloc(sizeof (struct narg));
		  n->type = NARG;
		  n->narg.text = wordtext;
		  n->narg.backquote = backquotelist;
		  *app = n;
		  app = &n->narg.next;
	    } else if (lasttoken == TREDIR) {
		  *rpp = n = redirnode;
		  rpp = &n->nfile.next;
		  parsefname();	/* read name of redirection file */
	    } else if (lasttoken == TLP && app == &args->narg.next
				 && rpp == &redir) {
		  /* We have a function */
		  if (readtoken() != TRP)
			synexpect(TRP);
		  if (! goodname(n->narg.text))
			synerror("Bad function name");
		  n->type = NDEFUN;
		  n->narg.next = command();
		  return n;
	    } else {
		  tokpushback++;
		  break;
	    }
      }
      *app = NULL;
      *rpp = NULL;
      n = (union node *)stalloc(sizeof (struct ncmd));
      n->type = NCMD;
      n->ncmd.backgnd = 0;
      n->ncmd.args = args;
      n->ncmd.redirect = redir;
      return n;
}


STATIC void
parsefname() {
      union node *n = redirnode;

      if (readtoken() != TWORD)
	    synexpect(-1);
      if (n->type == NHERE) {
	    struct heredoc *here = heredoc;
	    struct heredoc *p;
	    int i;

	    if (quoteflag == 0)
		  n->type = NXHERE;
	    TRACE(("Here document %d\n", n->type));
	    if (here->striptabs) {
		  while (*wordtext == '\t')
			wordtext++;
	    }
	    if (! noexpand(wordtext) || (i = strlen(wordtext)) == 0 || i > EOFMARKLEN)
		  synerror("Illegal eof marker for << redirection");
	    rmescapes(wordtext);
	    here->eofmark = wordtext;
	    here->next = NULL;
	    if (heredoclist == NULL)
		  heredoclist = here;
	    else {
		  for (p = heredoclist ; p->next ; p = p->next);
		  p->next = here;
	    }
      } else if (n->type == NTOFD || n->type == NFROMFD) {
	    if (is_digit(wordtext[0]))
		  n->ndup.dupfd = digit_val(wordtext[0]);
	    else if (wordtext[0] == '-')
		  n->ndup.dupfd = -1;
	    else
		  goto bad;
	    if (wordtext[1] != '\0') {
bad:
		  synerror("Bad fd number");
	    }
      } else {
	    n->nfile.fname = (union node *)stalloc(sizeof (struct narg));
	    n = n->nfile.fname;
	    n->type = NARG;
	    n->narg.next = NULL;
	    n->narg.text = wordtext;
	    n->narg.backquote = backquotelist;
      }
}


/*
 * Input any here documents.
 */

STATIC void
parseheredoc() {
      struct heredoc *here;
      union node *n;

      while (heredoclist) {
	    here = heredoclist;
	    heredoclist = here->next;
	    if (needprompt) {
		  putprompt(ps2val());
		  needprompt = 0;
	    }
	    readtoken1(pgetc(), here->here->type == NHERE? SQSYNTAX : DQSYNTAX,
			here->eofmark, here->striptabs);
	    n = (union node *)stalloc(sizeof (struct narg));
	    n->narg.type = NARG;
	    n->narg.next = NULL;
	    n->narg.text = wordtext;
	    n->narg.backquote = backquotelist;
	    here->here->nhere.doc = n;
      }
}



/*
 * This routine is called to tell readtoken that we are at the beginning
 * of a command, so newlines should be ignored and keywords should be
 * checked for.  We munge things here rather than setting a flag for
 * readtoken.
 */

STATIC void
checkkwd() {
      register char *const *pp;
      int t;

      while ((t = readtoken()) == TNL)
	    parseheredoc();
      if (t == TWORD && quoteflag == 0) {
	    for (pp = parsekwd ; *pp ; pp++) {
		  if (**pp == *wordtext && equal(*pp, wordtext)) {
			lasttoken = pp - parsekwd + KWDOFFSET;
			break;
		  }
	    }
      }
      tokpushback++;
}



STATIC int xxreadtoken();

STATIC int
readtoken() {
      int t;

      if (tokpushback) {
	    return xxreadtoken();
      } else {
	    t = xxreadtoken();
	    /* TRACE(("token %s\n", tokname[t])); */
	    return t;
      }
}


/*
 * Read the next input token.
 * If the token is a word, we set backquotelist to the list of cmds in
 *	backquotes.  We set quoteflag to true if any part of the word was
 *	quoted.
 * If the token is TREDIR, then we set redirnode to a structure containing
 *	the redirection.
 * In all cases, the variable startlinno is set to the number of the line
 *	on which the token starts.
 *
 * [Change comment:  here documents and internal procedures]
 * [Readtoken shouldn't have any arguments.  Perhaps we should make the
 *  word parsing code into a separate routine.  In this case, readtoken
 *  doesn't need to have any internal procedures, but parseword does.
 *  We could also make parseoperator in essence the main routine, and
 *  have parseword (readtoken1?) handle both words and redirection.]
 */

#define RETURN(token)	return lasttoken = token

STATIC int
xxreadtoken() {
      register c;

      if (tokpushback) {
	    tokpushback = 0;
	    return lasttoken;
      }
      if (needprompt) {
	    putprompt(ps2val());
	    needprompt = 0;
      }
      startlinno = plinno;
      for (;;) {	/* until token or start of word found */
	    c = pgetc_macro();
	    if (c == ' ' || c == '\t')
		  continue;		/* quick check for white space first */
	    switch (c) {
	    case ' ': case '\t':
		  continue;
	    case '#':
		  while ((c = pgetc()) != '\n' && c != PEOF);
		  pungetc();
		  continue;
	    case '\\':
		  if (pgetc() == '\n') {
			startlinno = ++plinno;
			if (doprompt)
			      putprompt(ps2val());
			continue;
		  }
		  pungetc();
		  goto breakloop;
	    case '\n':
		  plinno++;
		  needprompt = doprompt;
		  RETURN(TNL);
	    case PEOF:
		  RETURN(TEOF);
	    case '&':
		  if (pgetc() == '&')
			RETURN(TAND);
		  pungetc();
		  RETURN(TBACKGND);
	    case '|':
		  if (pgetc() == '|')
			RETURN(TOR);
		  pungetc();
		  RETURN(TPIPE);
	    case ';':
		  if (pgetc() == ';')
			RETURN(TENDCASE);
		  pungetc();
		  RETURN(TSEMI);
	    case '(':
		  RETURN(TLP);
	    case ')':
		  RETURN(TRP);
	    default:
		  goto breakloop;
	    }
      }
breakloop:
      return readtoken1(c, BASESYNTAX, (char *)NULL, 0);
#undef RETURN
}



/*
 * If eofmark is NULL, read a word or a redirection symbol.  If eofmark
 * is not NULL, read a here document.  In the latter case, eofmark is the
 * word which marks the end of the document and striptabs is true if
 * leading tabs should be stripped from the document.  The argument firstc
 * is the first character of the input token or document.
 *
 * Because C does not have internal subroutines, I have simulated them
 * using goto's to implement the subroutine linkage.  The following macros
 * will run code that appears at the end of readtoken1.
 */

#define CHECKEND()	{goto checkend; checkend_return:;}
#define PARSEREDIR()	{goto parseredir; parseredir_return:;}
#define PARSESUB()	{goto parsesub; parsesub_return:;}
#define PARSEBACKQOLD()	{oldstyle = 1; goto parsebackq; parsebackq_oldreturn:;}
#define PARSEBACKQNEW()	{oldstyle = 0; goto parsebackq; parsebackq_newreturn:;}

STATIC int
readtoken1(firstc, syntax, eofmark, striptabs)
      int firstc;
      char const *syntax;
      char *eofmark;
      int striptabs;
      {
      register c = firstc;
      register char *out;
      int len;
      char line[EOFMARKLEN + 1];
      struct nodelist *bqlist;
      int quotef;
      int dblquote;
      int varnest;
      int oldstyle;

      startlinno = plinno;
      dblquote = 0;
      if (syntax == DQSYNTAX)
	    dblquote = 1;
      quotef = 0;
      bqlist = NULL;
      varnest = 0;
      STARTSTACKSTR(out);
      loop: {	/* for each line, until end of word */
#if ATTY
	    if (c == '\034' && doprompt
	     && attyset() && ! equal(termval(), "emacs")) {
		  attyline();
		  if (syntax == BASESYNTAX)
			return readtoken();
		  c = pgetc();
		  goto loop;
	    }
#endif
	    CHECKEND();	/* set c to PEOF if at end of here document */
	    for (;;) {	/* until end of line or end of word */
		  CHECKSTRSPACE(3, out);	/* permit 3 calls to USTPUTC */
		  switch(syntax[c]) {
		  case CNL:	/* '\n' */
			if (syntax == BASESYNTAX)
			      goto endword;	/* exit outer loop */
			USTPUTC(c, out);
			plinno++;
			if (doprompt) {
			      putprompt(ps2val());
			}
			c = pgetc();
			goto loop;		/* continue outer loop */
		  case CWORD:
			USTPUTC(c, out);
			break;
		  case CCTL:
			if (eofmark == NULL || dblquote)
			      USTPUTC(CTLESC, out);
			USTPUTC(c, out);
			break;
		  case CBACK:	/* backslash */
			c = pgetc();
			if (c == PEOF) {
			      USTPUTC('\\', out);
			      pungetc();
			} else if (c == '\n') {
			      if (doprompt)
				    putprompt(ps2val());
			} else {
			      if (dblquote && c != '\\' && c != '`' && c != '$'
					   && (c != '"' || eofmark != NULL))
				    USTPUTC('\\', out);
			      if (SQSYNTAX[c] == CCTL)
				    USTPUTC(CTLESC, out);
			      USTPUTC(c, out);
			      quotef++;
			}
			break;
		  case CSQUOTE:
			syntax = SQSYNTAX;
			break;
		  case CDQUOTE:
			syntax = DQSYNTAX;
			dblquote = 1;
			break;
		  case CENDQUOTE:
			if (eofmark) {
			      USTPUTC(c, out);
			} else {
			      syntax = BASESYNTAX;
			      quotef++;
			      dblquote = 0;
			}
			break;
		  case CVAR:	/* '$' */
			PARSESUB();		/* parse substitution */
			break;
		  case CENDVAR:	/* '}' */
			if (varnest > 0) {
			      varnest--;
			      USTPUTC(CTLENDVAR, out);
			} else {
			      USTPUTC(c, out);
			}
			break;
		  case CBQUOTE:	/* '`' */
			if (parsebackquote && syntax == BASESYNTAX) {
			      if (out == stackblock())
				    return lasttoken = TENDBQUOTE;
			      else
				    goto endword;	/* exit outer loop */
			}
			PARSEBACKQOLD();
			break;
		  case CEOF:
			goto endword;		/* exit outer loop */
		  default:
			if (varnest == 0)
			      goto endword;	/* exit outer loop */
			USTPUTC(c, out);
		  }
		  c = pgetc_macro();
	    }
      }
endword:
      if (syntax != BASESYNTAX && eofmark == NULL)
	    synerror("Unterminated quoted string");
      if (varnest != 0) {
	    startlinno = plinno;
	    synerror("Missing '}'");
      }
      USTPUTC('\0', out);
      len = out - stackblock();
      out = stackblock();
      if (eofmark == NULL) {
	    if ((c == '>' || c == '<')
	     && quotef == 0
	     && len <= 2
	     && (*out == '\0' || is_digit(*out))) {
		  PARSEREDIR();
		  return lasttoken = TREDIR;
	    } else {
		  pungetc();
	    }
      }
      quoteflag = quotef;
      backquotelist = bqlist;
      grabstackblock(len);
      wordtext = out;
      return lasttoken = TWORD;
/* end of readtoken routine */



/*
 * Check to see whether we are at the end of the here document.  When this
 * is called, c is set to the first character of the next input line.  If
 * we are at the end of the here document, this routine sets the c to PEOF.
 */

checkend: {
      if (eofmark) {
	    if (striptabs) {
		  while (c == '\t')
			c = pgetc();
	    }
	    if (c == *eofmark) {
		  if (pfgets(line, sizeof line) != NULL) {
			register char *p, *q;

			p = line;
			for (q = eofmark + 1 ; *q && *p == *q ; p++, q++);
			if (*p == '\n' && *q == '\0') {
			      c = PEOF;
			      plinno++;
			      needprompt = doprompt;
			} else {
			      ppushback(line, strlen(line));
			}
		  }
	    }
      }
      goto checkend_return;
}


/*
 * Parse a redirection operator.  The variable "out" points to a string
 * specifying the fd to be redirected.  The variable "c" contains the
 * first character of the redirection operator.
 */

parseredir: {
      char fd = *out;
      union node *np;

      np = (union node *)stalloc(sizeof (struct nfile));
      if (c == '>') {
	    np->nfile.fd = 1;
	    c = pgetc();
	    if (c == '>')
		  np->type = NAPPEND;
	    else if (c == '&')
		  np->type = NTOFD;
	    else {
		  np->type = NTO;
		  pungetc();
	    }
      } else {	/* c == '<' */
	    np->nfile.fd = 0;
	    c = pgetc();
	    if (c == '<') {
		  if (sizeof (struct nfile) != sizeof (struct nhere)) {
			np = (union node *)stalloc(sizeof (struct nhere));
			np->nfile.fd = 0;
		  }
		  np->type = NHERE;
		  heredoc = (struct heredoc *)stalloc(sizeof (struct heredoc));
		  heredoc->here = np;
		  if ((c = pgetc()) == '-') {
			heredoc->striptabs = 1;
		  } else {
			heredoc->striptabs = 0;
			pungetc();
		  }
	    } else if (c == '&')
		  np->type = NFROMFD;
	    else {
		  np->type = NFROM;
		  pungetc();
	    }
      }
      if (fd != '\0')
	    np->nfile.fd = digit_val(fd);
      redirnode = np;
      goto parseredir_return;
}


/*
 * Parse a substitution.  At this point, we have read the dollar sign
 * and nothing else.
 */

parsesub: {
      int subtype;
      int typeloc;
      int flags;
      char *p;
#ifndef GDB_HACK
      static const char types[] = "}-+?=";
#endif

      c = pgetc();
      if (c == ' ' || c == '\t' || c == '\n' || c == '"' || c == PEOF) {
	    USTPUTC('$', out);
	    pungetc();
      } else if (c == '(') {	/* $(command) */
	    PARSEBACKQNEW();
      } else {
	    USTPUTC(CTLVAR, out);
	    typeloc = out - stackblock();
	    USTPUTC(VSNORMAL, out);
	    subtype = VSNORMAL;
	    if (c == '{') {
		  c = pgetc();
		  subtype = 0;
	    }
	    if (is_name(c)) {
		  do {
			STPUTC(c, out);
			c = pgetc();
		  } while (is_in_name(c));
	    } else {
		  if (! is_special(c))
badsub:			synerror("Bad substitution");
		  USTPUTC(c, out);
		  c = pgetc();
	    }
	    STPUTC('=', out);
	    flags = 0;
	    if (subtype == 0) {
		  if (c == ':') {
			flags = VSNUL;
			c = pgetc();
		  }
		  p = strchr(types, c);
		  if (p == NULL)
			goto badsub;
		  subtype = p - types + VSNORMAL;
	    } else {
		  pungetc();
	    }
	    if (dblquote)
		  flags |= VSQUOTE;
	    *(stackblock() + typeloc) = subtype | flags;
	    if (subtype != VSNORMAL)
		  varnest++;
      }
      goto parsesub_return;
}


/*
 * Called to parse command substitutions.  Newstyle is set if the command
 * is enclosed inside $(...); nlpp is a pointer to the head of the linked
 * list of commands (passed by reference), and savelen is the number of
 * characters on the top of the stack which must be preserved.
 */

parsebackq: {
      struct nodelist **nlpp;
      int savepbq;
      union node *n;
      char *volatile str;
      struct jmploc jmploc;
      struct jmploc *volatile savehandler;
      int savelen;
      int t;

      savepbq = parsebackquote;
      if (setjmp(jmploc.loc)) {
	    if (str)
		  ckfree(str);
	    parsebackquote = 0;
	    handler = savehandler;
	    longjmp(handler, 1);
      }
      INTOFF;
      str = NULL;
      savelen = out - stackblock();
      if (savelen > 0) {
	    str = ckmalloc(savelen);
	    bcopy(stackblock(), str, savelen);
      }
      savehandler = handler;
      handler = &jmploc;
      INTON;
      nlpp = &bqlist;
      while (*nlpp)
	    nlpp = &(*nlpp)->next;
      *nlpp = (struct nodelist *)stalloc(sizeof (struct nodelist));
      (*nlpp)->next = NULL;
      parsebackquote = oldstyle;
      n = list(0);
      t = oldstyle? TENDBQUOTE : TRP;
      if (readtoken() != t)
	    synexpect(t);
      (*nlpp)->n = n;
      while (stackblocksize() <= savelen)
	    growstackblock();
      STARTSTACKSTR(out);
      if (str) {
	    bcopy(str, out, savelen);
	    STADJUST(savelen, out);
	    INTOFF;
	    ckfree(str);
	    str = NULL;
	    INTON;
      }
      parsebackquote = savepbq;
      handler = savehandler;
      USTPUTC(CTLBACKQ + dblquote, out);
      if (oldstyle)
	    goto parsebackq_oldreturn;
      else
	    goto parsebackq_newreturn;
}

} /* end of readtoken */



#ifdef mkinit
RESET {
      tokpushback = 0;
}
#endif


#if ATTY
/*
 * Called to process a command generated by atty.  We execute the line,
 * and catch any errors that occur so they don't propagate outside of
 * this routine.
 */

STATIC void
attyline() {
      char line[256];
      struct stackmark smark;
      struct jmploc jmploc;
      struct jmploc *volatile savehandler;

      if (pfgets(line, sizeof line) == NULL)
	    return;				/* "can't happen" */
      if (setjmp(jmploc.loc)) {
	    if (exception == EXERROR)
		  out2str("\033]D\n");
	    handler = savehandler;
	    longjmp(handler, 1);
      }
      savehandler = handler;
      handler = &jmploc;
      setstackmark(&smark);
      evalstring(line);
      popstackmark(&smark);
      handler = savehandler;
      doprompt = 1;
}


/*
 * Output a prompt for atty.  We output the prompt as part of the
 * appropriate escape sequence.  
 */

STATIC void
putprompt(s)
      char *s;
      {
      register char *p;

      if (attyset() && ! equal(termval(), "emacs")) {
	    if (strchr(s, '\7'))
		  out2c('\7');
	    out2str("\033]P1;");
	    for (p = s ; *p ; p++) {
		  if ((unsigned)(*p - ' ') <= '~' - ' ')
			out2c(*p);
	    }
	    out2c('\n');
      } else {
	    out2str(s);
      }
}
#endif



/*
 * Returns true if the text contains nothing to expand (no dollar signs
 * or backquotes).
 */

STATIC int
noexpand(text)
      char *text;
      {
      register char *p;
      register char c;

      p = text;
      while ((c = *p++) != '\0') {
	    if (c == CTLESC)
		  p++;
	    else if (BASESYNTAX[c] == CCTL)
		  return 0;
      }
      return 1;
}


/*
 * Return true if the argument is a legal variable name (a letter or
 * underscore followed by zero or more letters, underscores, and digits).
 */

int
goodname(name)
      char *name;
      {
      register char *p;

      p = name;
      if (! is_name(*p))
	    return 0;
      while (*++p) {
	    if (! is_in_name(*p))
		  return 0;
      }
      return 1;
}


/*
 * Called when an unexpected token is read during the parse.  The argument
 * is the token that is expected, or -1 if more than one type of token can
 * occur at this point.
 */

STATIC void
synexpect(token) {
      char msg[64];

      if (token >= 0) {
	    fmtstr(msg, 64, "%s unexpected (expecting %s)",
		  tokname[lasttoken], tokname[token]);
      } else {
	    fmtstr(msg, 64, "%s unexpected", tokname[lasttoken]);
      }
      synerror(msg);
}


STATIC void
synerror(msg)
      char *msg;
      {
      if (commandname)
	    outfmt(&errout, "%s: %d: ", commandname, startlinno);
      outfmt(&errout, "Syntax error: %s\n", msg);
      error((char *)NULL);
}
