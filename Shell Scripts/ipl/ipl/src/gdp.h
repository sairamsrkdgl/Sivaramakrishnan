
#include "gdp.d"
char Chunk[INLENGTH][INWIDTH]; 
char Ichunk[INLENGTH][INWIDTH];
int Ilines, Clines;
char Arg[MAXARGS][ARGLEN+1];
int Argc;
int Do_cons_check;
FILE *Sfp;
char Last_constraint[80];
