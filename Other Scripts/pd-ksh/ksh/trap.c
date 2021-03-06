/*
 * signal handling
 */

static char *RCSid = "$Header: trap.c,v 3.1 88/11/03 09:18:00 egisin Exp $";

#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <setjmp.h>
#include "sh.h"

Trap sigtraps [SIGNALS] = {
	{0,	"EXIT", "Signal 0"}, /* todo: belongs in e.loc->exit */
	{SIGHUP, "HUP", "Hangup"},
	{SIGINT, "INT", "Interrupt"},
	{SIGQUIT, "QUIT", "Quit"},
	{SIGILL, "ILL", "Illegal instruction"},
	{SIGTRAP, "TRAP", "Trace trap"},
	{SIGIOT, "IOT", "Abort"},
	{SIGEMT, "EMT", "EMT trap"},
	{SIGFPE, "FPE", "Floating exception"},
	{SIGKILL, "KILL", "Killed"},
	{SIGBUS, "BUS", "Bus error"},
	{SIGSEGV, "SEGV", "Memory fault"},
	{SIGSYS, "SYS", "Bad system call"},
	{SIGPIPE, "PIPE", "Broken pipe"},
	{SIGALRM, "ALRM", "Alarm clock"},
	{SIGTERM, "TERM", "Terminated"},
#if JOBS			/* todo: need to be more portable */
	{SIGURG, "URG", "Urgent condition"}, /* BSDism */
	{SIGSTOP, "STOP", "Stop (signal)"},
	{SIGTSTP, "TSTP", "Stop"},
	{SIGCONT, "CONT", "Continue"},
	{SIGCHLD, "CHLD", "Waiting children"},
	{SIGTTIN, "TTIN", "Stop (tty input)"},
	{SIGTTOU, "TTOU", "Stop (tty output)"},
#endif
};

Trap *
gettrap(name)
	char *name;
{
	int i;
	register Trap *p;

	if (digit(*name)) {
		i = getn(name);
		return (0 <= i && i < SIGNALS) ? &sigtraps[getn(name)] : NULL;
	}
#if 0
	if (strcmp("ERR", name) == 0)
		return &e.loc->err;
	if (strcmp("EXIT", name) == 0)
		return &e.loc->exit;
#endif
	for (p = sigtraps, i = SIGNALS; --i >= 0; p++)
		if (strcmp(p->name, name) == 0)
			return p;
	return NULL;
}

/*
 * trap signal handler
 */
void
trapsig(i)
	int i;
{
	trap = sigtraps[i].set = 1;
	if (i == SIGINT && e.type == E_PARSE)
		/* dangerous but necessary to deal with BSD silly signals */
		longjmp(e.jbuf, 1);
	(void) signal(i, trapsig); /* todo: use sigact */
}

/*
 * run any pending traps
 */
runtraps()
{
	int i;
	register Trap *p;

	for (p = sigtraps, i = SIGNALS; --i >= 0; p++)
		if (p->set)
			runtrap(p);
	trap = 0;
}

runtrap(p)
	Trap *p;
{
	char *trapstr;

	p->set = 0;
	if ((trapstr = p->trap) == NULL)
		if (p->signal == SIGINT)
			unwind();	/* return to shell() */
		else
			return;
	if (p->signal == 0)	/* ??? */
		p->trap = 0;
	command(trapstr);
}
 
/* restore signals for children */
cleartraps()
{
	int i;
	register Trap *p;

	for (i = SIGNALS, p = sigtraps; --i >= 0; p++) {
		p->set = 0;
		if (p->ourtrap && signal(p->signal, SIG_IGN) != SIG_IGN)
			(void) signal(p->signal, SIG_DFL);
	}
}

ignoresig(i)
	int i;
{
	if (signal(i, SIG_IGN) != SIG_IGN)
		sigtraps[i].sig_dfl = 1;
}

restoresigs()
{
	int i;
	register Trap *p;

	for (p = sigtraps, i = SIGNALS; --i >= 0; p++)
		if (p->sig_dfl) {
			p->sig_dfl = 0;
			(void) signal(p->signal, SIG_DFL);
		}
}

