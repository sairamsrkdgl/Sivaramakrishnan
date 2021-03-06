/*
 * lexical analysis and source input
 */

static char *RCSid = "$Header: lex.c,v 3.1 88/11/03 09:16:43 egisin Exp $";

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <setjmp.h>
#include <unistd.h>
#include "sh.h"
#include "lex.h"
#include "tree.h"
#include "table.h"
#include "expand.h"

	int	ttyfd = -1;		/* tty fd for edit and jobs */
	char   *history[HISTORY];	/* saved commands */
	char  **histptr = history - 1;	/* last history item */
	int	histpush;		/* number of pushed fc commands */

static	int	alias;
static	int	getsc_ ARGS((void));

/* optimized getsc_() */
#define	getsc()	((*source->str != 0) ? *source->str++ : getsc_())
#define	ungetsc() (source->str--)

/*
 * Lexical analyzer
 *
 * tokens are not regular expressions, they are LL(1).
 * for example, "${var:-${PWD}}", and "$(size $(whence ksh))".
 * hence the state stack.
 */

int
yylex(cf)
	int cf;
{
	register int c, state;
	char states [64], *statep = states;
	XString ws;		/* expandable output word */
	register char *wp;	/* output word pointer */
	register char *sp, *dp;
	int istate;
	int c2;

  Again:
	Xinit(ws, wp, 256);
	if (alias) {			/* trailing ' ' in alias definition */
		alias = 0;
		cf |= ALIAS;
	}

	if (cf&ONEWORD)
		istate = SWORD;
	else {			/* normal lexing */
		istate = SBASE;
		while ((c = getsc()) == ' ' || c == '\t')
			;
		if (c == '#')
			while ((c = getsc()) != 0 && c != '\n')
				;
		ungetsc();
	}

	/* collect non-special or quoted characters to form word */
	for (*statep = state = istate;
	     !((c = getsc()) == 0 || state == SBASE && ctype(c, C_LEX1)); ) {
		Xcheck(ws, wp);
		switch (state) {
		  case SBASE:
		  Sbase:
			switch (c) {
			  case '\\':
				c = getsc();
				if (c != '\n')
					*wp++ = QCHAR, *wp++ = c;
				else
					if (wp == Xstring(ws, wp))
						goto Again;
				break;
			  case '\'':
				*++statep = state = SSQUOTE;
				*wp++ = OQUOTE;
				break;
			  case '"':
				*++statep = state = SDQUOTE;
				*wp++ = OQUOTE;
				break;
			  default:
				goto Subst;
			}
			break;

		  Subst:
			switch (c) {
			  case '\\':
				c = getsc();
				switch (c) {
				  case '\n':
					break;
				  case '"': case '\\':
				  case '$': case '`':
					*wp++ = QCHAR, *wp++ = c;
					break;
				  default:
					*wp++ = CHAR, *wp++ = '\\';
					*wp++ = CHAR, *wp++ = c;
					break;
				}
				break;
			  case '$':
				c = getsc();
				if (c == '(') {
					*++statep = state = SPAREN;
					*wp++ = COMSUB;
				} else
				if (c == '{') {
					*++statep = state = SBRACE;
					*wp++ = OSUBST;
					c = getsc();
					do {
						Xcheck(ws, wp);
						*wp++ = c;
						c = getsc();
					} while (ctype(c, C_ALPHA|C_DIGIT));
					*wp++ = 0;
					/* todo: more compile-time checking */
					if (c == '}')
						ungetsc();
					else if (c == '#' || c == '%') {
						/* Korn pattern trimming */
						if (getsc() == c)
							c |= 0x80;
						else
							ungetsc();
						*wp++ = c;
					} else if (c == ':')
						*wp++ = 0x80|getsc();
					else
						*wp++ = c;
				} else if (ctype(c, C_ALPHA)) {
					*wp++ = OSUBST;
					do {
						*wp++ = c;
						c = getsc();
					} while (ctype(c, C_ALPHA|C_DIGIT));
					*wp++ = 0;
					*wp++ = CSUBST;
					ungetsc();
				} else if (ctype(c, C_DIGIT|C_VAR1)) {
					*wp++ = OSUBST;
					*wp++ = c;
					*wp++ = 0;
					*wp++ = CSUBST;
				} else {
					*wp++ = CHAR, *wp++ = '$';
					*wp++ = CHAR, *wp++ = c;
				}
				break;
			  case '`':
				*++statep = state = SBQUOTE;
				*wp++ = COMSUB;
				break;
			  default:
				*wp++ = CHAR, *wp++ = c;
			}
			break;

		  case SSQUOTE:
			if (c == '\'') {
				state = *--statep;
				*wp++ = CQUOTE;
			} else
				*wp++ = QCHAR, *wp++ = c;
			break;

		  case SDQUOTE:
			if (c == '"') {
				state = *--statep;
				*wp++ = CQUOTE;
			} else
				goto Subst;
			break;

		  case SPAREN:
			if (c == '(')
				*++statep = state;
			else if (c == ')')
				state = *--statep;
			if (state == SPAREN)
				*wp++ = c;
			else
				*wp++ = 0; /* end of COMSUB */
			break;

		  case SBRACE:
			if (c == '}') {
				state = *--statep;
				*wp++ = CSUBST;
			} else
				goto Sbase;
			break;

		  case SBQUOTE:
			if (c == '`') {
				*wp++ = 0;
				state = *--statep;
			} else	/* todo: handle silly `\`` escapes */
				/* todo: both \" and \` in "`...`" */
				*wp++ = c;
			break;

		  case SWORD:	/* ONEWORD */
			goto Subst;
		}
	}
	if (state != istate)
		yyerror("no closing quote");

	if (c == '<' || c == '>') {
		char *cp = Xstring(ws, wp);
		if (wp > cp && cp[0] == CHAR && digit(cp[1])) {
			wp = cp; /* throw away word */
			iounit = cp[1] - '0';
		} else
			iounit = -1; /* default */
	}

	if (wp == Xstring(ws, wp) && state == SBASE) {
		Xfree(ws, sp);	/* free word */
		/* no word, process LEX1 character */
		switch (c) {
		  default:
			return c;

		  case '|':
		  case '&':
		  case ';':
			if (getsc() == c)
				c = (c == ';') ? BREAK :
				    (c == '|') ? LOGOR :
				    (c == '&') ? LOGAND :
				    YYERRCODE;
			else
				ungetsc();
			return c;

		  case '>':
		  case '<':
			c2 = getsc();
			if (c2 == '>' || c2 == '<') {
				if (c2 != c)
					yyerror("syntax error");
				yylval.i = c == '>'? IOWRITE|IOCAT: IOHERE;
				c2 = getsc();
			} else
				yylval.i = c == '>'? IOWRITE: IOREAD;
			if (c2 != '&' || yylval.i == IOHERE)
				ungetsc();
			else
				yylval.i |= IODUP;
			return REDIR;

		  case '\n':
			gethere();
			if (cf & CONTIN)
				goto Again;
			return c;

		  case '(':
			c2 = getsc();
			if (c2 == ')')
				c = MPAREN;
			else if (c2 == '(')
				yyerror("(( not supported");
			else
				ungetsc();
		  case ')':
			return c;
		}
	}

	*wp++ = EOS;		/* terminate word */
	yylval.cp = Xclose(ws, wp);
	if (state == SWORD)	/* ONEWORD? */
		return LWORD;
	ungetsc();		/* unget terminator */

	/* copy word to unprefixed string ident */
	for (sp = yylval.cp, dp = ident; dp < ident+IDENT && (c = *sp++) == CHAR; )
		*dp++ = *sp++;
	*dp = 0;
#if 0
	if (*ident == '~' || (dp = strchr(ident, '=')) != NULL && dp[1] == '~')
		"Tilde expansion";
#endif
	if (c != EOS)
		*ident = 0;	/* word is not unquoted */

	if (*ident != 0 && (cf&(KEYWORD|ALIAS))) {
		register struct tbl *p;

		p = tsearch(&lexicals, ident, hash(ident));
		if (p != NULL && (p->flag&ISSET))
			if (p->type == CKEYWD && (cf&KEYWORD)) {
				afree(yylval.cp, ATEMP);
				return p->val.i;
			} else
			if (p->type == CALIAS && (cf&ALIAS)) {
				register Source *s;

				/* check for recursive aliasing */
				for (s = source; s->type == SALIAS; s = s->next)
					if (s->u.tblp == p)
						return LWORD;
				afree(yylval.cp, ATEMP);

				/* push alias expansion */
				s = pushs(SALIAS);
				s->str = p->val.s;
				s->u.tblp = p;
				s->next = source;
				source = s;
				goto Again;
			}
	}

	return LWORD;
}

static void readhere();

gethere()
{
	register struct ioword **p;

	for (p = heres; p < herep; p++)
		readhere(*p);
	herep = heres;
}

/*
 * read "<<word" text into temp file
 * todo: set up E_ERR to fclose(f) on unwind
 */

static void
readhere(iop)
	register struct ioword *iop;
{
	register FILE *f;
	struct temp *h;
	register int c;
	char *eof;
	register char *cp;
	int strip;
	char line [LINE+1];

	eof = evalstr(iop->name, 0);
	strip = *eof == '-';
	eof += strip;		/* skip '-' */

	h = maketemp(ATEMP);
	h->next = e.temps; e.temps = h;
	iop->name = h->name;
	f = fopen(h->name, "w");
	if (f == NULL)
		errorf("Cannot create temporary file\n");
	setvbuf(f, (char *)NULL, _IOFBF, BUFSIZ);

	for (;;) {
		cp = line;
		while ((c = getsc()) != '\n') {
			if (c == 0)
				errorf("here document `%s' unclosed\n", eof);
			if (cp >= line+LINE)
				break;
			*cp++ = c;
		}
		ungetsc();
		*cp = 0;
		for (cp = line; strip && *cp == '\t'; cp++)
			;
		if (strcmp(eof, cp) == 0 || c == 0)
			break;
		while ((c = *cp++) != '\0')
			putc(c, f);
		while ((c = getsc()) != '\n') {
			if (c == 0)
				errorf("here document `%s' unclosed\n", eof);
			putc(c, f);
		}
		putc(c, f);
	}
	fclose(f);
}

void
yyerror(msg)
	Const char *msg;
{
	yynerrs++;
	while (source->type == SALIAS) /* pop aliases */
		source = source->next;
	if (source->file != NULL)
		shellf("%s[%d]: ", source->file, source->line);
	source->str = null;	/* zap pending input */
	errorf("%s\n", msg);
}

/*
 * input for yylex with alias expansion
 */

Source *
pushs(type)
	int type;
{
	register Source *s;

	s = (Source *) alloc(sizeof(Source), ATEMP);
	s->type = type;
	s->str = null;		/* "" */
	s->line = 0;
	s->file = NULL;
	s->echo = 0;
	s->next = NULL;
	return s;
}

int
getsc_()
{
	register Source *s = source;
	register int c;

	while ((c = *s->str++) == 0) {
		s->str = NULL;		/* return 0 for EOF by default */
		switch (s->type) {
		  case SEOF:
			s->str = null;
			return 0;

		  case STTY:
			if (histpush < 0) {	/* commands pushed by dofc */
				s->type = SHIST;
				s->str = null;
				continue;
			}
			s->line++;
			s->str = line;
			line[0] = '\0';
			pprompt(prompt);
			flushshf(1);	flushshf(2);
#if EDIT
			if (flag[FEMACS])
				c = x_read(ttyfd, line, LINE);
			else
#endif
				c = read(ttyfd, line, LINE);
			if (c < 0) 	/* read error */
				c = 0;
			if (c == 0) /* EOF */
				s->str = NULL;
			prompt = strval(global("PS2"));
			line[c] = '\0';
			if (line[0] != '\n')
				histsave(line);
			else
				s->line--;
			break;

		  case SHIST:
			if (histpush == 0) {
				s->type = STTY;
				s->str = null;
				continue;
			}
			s->line++;
			s->str = histptr[++histpush];
			pprompt("!< ");	/* todo: PS9 */
			shellf("%s\n", s->str);
			strcpy(line, s->str);
			s->str = strchr(line, 0);
			*s->str++ = '\n';
			*s->str = 0;
			s->str = line;
			break;

		  case SFILE:
			s->line++;
			s->str = fgets(line, LINE, s->u.file);
			if (s->str == NULL)
				if (s->u.file != stdin)
					fclose(s->u.file);
			break;

		  case SWSTR:
			break;

		  case SSTRING:
			s->str = "\n";
			s->type = SEOF;
			break;

		  case SWORDS:
			s->str = *s->u.strv++;
			s->type = SWORDSEP;
			break;

		  case SWORDSEP:
			if (*s->u.strv == NULL) {
				s->str = "\n";
				s->type = SEOF;
			} else {
				s->str = " ";
				s->type = SWORDS;
			}
			break;

		  case SALIAS:
			s->str = s->u.tblp->val.s;
			if (s->str[0] != 0 && strchr(s->str, 0)[-1] == ' ')
				alias = 1;	/* trailing ' ' */
			source = s = s->next;	/* pop source stack */
			continue;
		}
		if (s->str == NULL) {
			s->type = SEOF;
			s->str = null; /* "" */
			return 0;
		}
		if (s->echo)
			fputs(s->str, shlout);
	}
	return c;
}

pprompt(cp)
	register char *cp;
{
	while (*cp != 0)
		if (*cp != '!')
			putc(*cp++, shlout);
		else
			if (*++cp == '!')
				putc(*cp++, shlout);
			else
				shellf("%d", source->line);
	fflush(shlout);
}

