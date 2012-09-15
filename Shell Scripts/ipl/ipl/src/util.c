#include "gdp.x"

extern char *getlogin();

/* ============== */
/* save_spec() can be called to save current proc specification in file s. */
save_parms( s )
char s[];
{
char fnm[PATHNAME_LEN];
FILE *fp;
int i;

sprintf( fnm, "%s/ipl%s%d.%s", TEMPDIR, getlogin(), getpid(), s );
fp = fopen( fnm, "w" );
if( fp == NULL ) { fprintf( stderr, "Can't w open inherit file %s.\n", fnm ); gdp_exit(); }
for( i = 0; i < Clines; i++ ) fprintf( fp, "%s", Chunk[i] );
fclose( fp );
}

/* =============== */
/* load_spec() can be called to use parameters from file s as defaults. */
load_parms( s )
char s[];
{
char fnm[PATHNAME_LEN];
FILE *fp;

if( strlen( s ) < 1 ) { Ilines = 0; return( 1 ); }
sprintf( fnm, "%s/ipl%s%d.%s", TEMPDIR, getlogin(), getpid(), s );
fp = fopen( fnm, "r" );
if( fp == NULL ) { fprintf( stderr, "Can't open inheritance file %s.\n", fnm ); gdp_exit(); }
Ilines = 0;
while( fgets( Ichunk[ Ilines ], INWIDTH, fp ) != NULL ) Ilines++;
fclose( fp );
}

/* ============== */
/* gdp_exit() can be used by application to explicitly do a clean exit */
gdp_exit()
{
char com[80];
sprintf( com, "rm %s/ipl%s%d* 2> /dev/null", TEMPDIR, getlogin(), getpid() );
system( com );
proc_call( "Exit" );
exit();
}

/* =============
   ============= */
/* The following are utilities for dealing with multi-line responses */
/* ========= */
/* getln() returns successive portions of s as separated by \n */

char *getln( s )
char s[];
{
static char rtn[INWIDTH];
static int c = 0;
int l, start;

/* a null argument resets */
if( strlen( s ) < 1 ) {
	c = 0;
	return( "" );
	}
start = c;
for( ; c < strlen( s ); c++ ) if( s[c] == '\n' ) break;
strncpy( rtn, &s[start], c-start );
rtn[ c-start ] = '\0';
c++;
return( rtn );
}


/* ======= */
/* countln() returns number of \n separated lines in s */

countln( s )
char s[];
{
int i, nnl;
if( strlen( s ) < 1 ) return( 0 );
nnl = 0;
for( i = 0; i < strlen( s ) -1; i++ ) if( s[i] == '\n' ) nnl++;
nnl++;
return( nnl );
}

/* ======== */
/* text_tofile() places text s into file f */

text_tofile( s, f )
char s[], f[];
{
FILE *fp;

fp = fopen( f, "w" );
if( fp == NULL ) { fprintf( stderr, "text_tofile: can't open Tempfile.\n" ); exit(); }
fprintf( fp, "%s\n", s );
fclose( fp );
}


/* ============ */
/* main program for testing these routines. */
/* main() 
{
char s[30];
char *getln();
strcpy( s, "Hello\nworld\nhello\n" );
printf( "%d\n", countln( s ) );
printf( "%s\n", getln( s ) );
printf( "%s\n", getln( s ) );
printf( "%s\n", getln( s ) );
strcpy( s, "Hello\nworld\nhello" );
printf( "%d\n", countln( s ) );
printf( "%s\n", getln( s ) );
printf( "%s\n", getln( s ) );
printf( "%s\n", getln( s ) );
text_tofile( s, "/usr/tmp/testutil001" );
}
*/
