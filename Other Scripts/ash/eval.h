extern char *commandname;	/* currently executing command */
extern int exitstatus;		/* exit status of last command */
extern struct strlist *cmdenviron;  /* environment for builtin command */


struct backcmd {		/* result of evalbackcmd */
      int fd;			/* file descriptor to read from */
      char *buf;		/* buffer */
      int nleft;		/* number of chars in buffer */
      struct job *jp;		/* job structure for command */
};


#ifdef __STDC__
void evalstring(char *);
void evaltree(union node *, int);
void evalbackcmd(union node *, struct backcmd *);
#else
void evalstring();
void evaltree();
void evalbackcmd();
#endif

/* in_function returns nonzero if we are currently evaluating a function */
#define in_function()	funcnest
extern int funcnest;
