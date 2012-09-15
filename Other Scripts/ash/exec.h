/* values of cmdtype */
#define CMDUNKNOWN -1		/* no entry in table for command */
#define CMDNORMAL 0		/* command is an executable program */
#define CMDBUILTIN 1		/* command is a shell builtin */
#define CMDFUNCTION 2		/* command is a shell function */


struct cmdentry {
      int cmdtype;
      union param {
	    int index;
	    union node *func;
      } u;
};


extern char *pathopt;		/* set by padvance */

#ifdef __STDC__
void shellexec(char **, char **, char *, int);
char *padvance(char **, char *);
void find_command(char *, struct cmdentry *, int);
int find_builtin(char *);
void hashcd(void);
void changepath(char *);
void defun(char *, union node *);
void unsetfunc(char *);
#else
void shellexec();
char *padvance();
void find_command();
int find_builtin();
void hashcd();
void changepath();
void defun();
void unsetfunc();
#endif
