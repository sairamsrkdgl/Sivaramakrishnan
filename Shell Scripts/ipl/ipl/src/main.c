/* GDP - Goal Driven Parameter mechanism.  
  This reads a user-supplied control file, fills Chunk[][] with the text
for an entire proc, may look for an inheritance file and fill Ichunk[][]
with that text.  It then sends control to application via proc_call().
The application then accesses paramters through the gget() call.
Gget looks first in Chunk, then in Ichunk, and finally in the Template
text to find a value.
  Also calls proc_call() for certain situations as follows:
Does a proc_call( "Initialize" ) at the very beginning;
Does a proc_call( "Endoffile" ) on end-of-file (control file);
Does a proc_call( "Exit" ) as its last action;
*/
#include <signal.h>
#include "gdp.h"

main( argc, argv )
int argc;
char *argv[];
{
int i;
int gdp_exit();

/* handle ^C */
signal( SIGINT, gdp_exit );


/* put args in globally-accessable structure */
Argc = argc;
if( argc > MAXARGS ) { fprintf( stderr, "Too many command line args (max=%d)\n", MAXARGS ); exit(); }
for( i = 0; i < argc; i++ ) {
	if( strlen( argv[i] ) > ARGLEN )
		{ fprintf( stderr, "Arg %d too long (max len=%d)\n", i, ARGLEN ); exit(); }

	strcpy( Arg[i], argv[i] );
	}


Do_cons_check = 0;

proc_call( "Initialize" ); /* note, vipl and mipl never return */

display();
gdp_exit();
}




/* ===================================================== */
/* Run display based on file descriptor Sfp.  Returns on eof */
display()
{

int 	i, 
	first, 		/* 1 during first line of control file, 0 otherwise */
	inheritflag;	/* 1 if cloning, 0 otherwise.  Used to make sure clones don't get cloned. */
char 	line[INWIDTH+5],  /* input line buffer */
	a[80], 		/* first token from input line */
	b[80],		/* second token from input line */
	proc[30],	/* current proc name */
	ans[10],
	clone[30];	/* clone or save name */



first = 1;
inheritflag = 0;
Clines = 0;


if( Sfp == NULL ) { fprintf( stderr, "Control file could not be found and/or opened.\n" ); exit(); }

/* loop on lines in control file.. */
while( 1 ) {
	a[0] = '\0'; b[0] = '\0'; /* null out tokens */
	if( fgets( line, INWIDTH+1, Sfp ) == NULL ) {
		if( strlen( line ) > INWIDTH ) {
			fprintf( stderr, "Control file line too long (max=%d).\n", INWIDTH );
			exit();
			}
		strcpy( a, "Proc" ); /* indicates last line to code below.. */
		fclose( Sfp );
		Sfp = NULL;
		}
	else sscanf( line, "%s %s", a, b );
	if( a[0] == '#' || a[0] == ':' ) continue;  /* comments */


	if( strcmp( a, "Proc" )==0 ) {  /* we have reached the end of proc, Chunk should be complete.. */

		if( !first ) {  /* process the proc whose text is now in Chunk */

			Ilines = 0;
			gget( clone, "Clone" );	/* get inheritance, if any */
			if( strlen( clone ) > 0 ) {
				inheritflag = 1;
				load_parms( clone );  /* fill Ichunk */
				}
			else 	{
				inheritflag = 0;
				load_parms( "" );
				}

			proc_call( proc ); /* call routines that do the work */

			
			if( !inheritflag ) {     /* save parms if desired, and if this is not a clone.. */
				gget( clone, "Saveas" );
				if( strlen( clone ) > 0 ) save_parms( clone ); /* write Ichunk out to a file */
				}
			}

		if( b[0] == '\0' ) { /* the fgets above returned NULL.. */
			proc_call( "Endoffile" );
			if( Sfp == NULL )break;  /* normal exit */
			else { first = 1; continue; }	/* user chose to restart */
			}


		Clines = 0; /* initialize chunk */
		strcpy( proc, b );
		proc[ strlen( proc )-1 ] = '\0';
		first = 0;
		}
	strcpy( Chunk[ Clines++ ], line ); /* add line to chunk */
	}
return( 1 );
}

