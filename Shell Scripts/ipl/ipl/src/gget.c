/* Given a parm name, returns a value.  Searches these places for the value:
   first: Chunk; second: Ichunk; third: templates.
   Handles " .." values which become command line args.
   Notes: 
	Multi-line values are returned with lines delimited by '\n'.
	All white space at beginning and end of value is stripped off.
	Master template values can be multi-line.
*/
#include <ctype.h>
#include "template.h"
#include "gdp.x"


gget( value, parm )
char value[], parm[];
{
int i, ix, ufound, mfound, inln, k, index, j, l;
char p[40], proc[40], constraint[80], foo[40], name[30];
static char linebuf[INWIDTH], lastproc[30]="";
static int start, stop;
inln = 0;

/* check user spec file */
if( Clines < 1 ) { fprintf( stderr, "gget: No chunk exists.\n" ); gdp_exit(); }

ufound = 0;
for( k = 0; k < Clines; k++ ) {      /* search Chunk for parm entry */
	inln++;
	if( sscanf( Chunk[k], "%s", p ) < 1 ) continue; /* blank lines */
	if( p[0] == '#' || p[0] == ':' ) continue; /* comments */
	if( !is_parm_name( p )) continue; 	/* non-parm lines */
	p[ strlen( p ) - 1 ] = '\0';    /* get rid of colon */

	if( strcmp( parm, p )==0 ) {  /* found the parm entry.. */
		ix = 0;
		getfld( linebuf, Chunk[k], &ix ); /* get first field-- not needed */
		strcpy( linebuf, &(Chunk[k][ix]) ); /* get value */
		strip_ws( linebuf );

		/* command line subst. */
		if( linebuf[0] == '$' ) cmdline_subst( linebuf );

		strcpy( value, linebuf );

		/* get additional lines, if any */
		while( k < Clines-1 ) {
			k++;
			strcpy( linebuf, Chunk[ k ] );
			strip_ws( linebuf );
			if( linebuf[0] == ':' || linebuf[0] == '#' ) continue;
			if( linebuf[0] == '$' ) cmdline_subst( linebuf );
			
			sscanf( linebuf, "%s", p );
			if( is_parm_name( p )) break;

			/* get rid of backslashes in text */
			for( j = 0, l = 0; j < strlen( linebuf ); j++ ) 
				if( linebuf[j] != '\134' ) linebuf[l++] = linebuf[j];
			linebuf[l] = '\0';

			if( strlen( value ) + strlen( linebuf ) > HBUFSIZ-1 ) {
				fprintf( stderr, "%s: multi-line text too long (max=%d).\n", parm, HBUFSIZ ); 
				gdp_exit(); 
				}
			else sprintf( value, "%s\n%s", value, linebuf );
			}
		ufound = 1;
		break;
		}
	}


/* if not found in user spec, and there is an inherit file defined, check there */
if( !ufound && Ilines > 0 ) {  
	for( k = 0; k < Ilines; k++ ) {
		sscanf( Ichunk[k], "%s", p );
		if( p[0] == '#' || p[0] == ':' ) continue;
		if( !is_parm_name( p )) continue;
		p[ strlen( p ) - 1 ] = '\0';/* get rid of colon */
		if( strcmp( parm, p )==0 ) {
			ix = 0;
			getfld( linebuf, Ichunk[k], &ix ); /* get first field-- not needed */
			strcpy( linebuf, &(Ichunk[k][ix]) ); /* get value */
			strip_ws( linebuf );
	
			/* command line subst. */
			if( linebuf[0] == '$' ) cmdline_subst( linebuf );

			strcpy( value, linebuf );
	
			/* get additional lines, if any */
			while( k < Ilines-1 ) {
				k++;
				strcpy( linebuf, Ichunk[ k ] );
				strip_ws( linebuf );
				if( linebuf[0] == ':' || linebuf[0] == '#' ) continue;
				if( linebuf[0] == '$' ) cmdline_subst( linebuf );
				
				sscanf( linebuf, "%s", p );
				if( is_parm_name( p )) break;

				/* get rid of backslashes in text */
				for( j = 0, l = 0; j < strlen( linebuf ); j++ ) 
					if( linebuf[j] != '\134' ) linebuf[l++] = linebuf[j];
				linebuf[l] = '\0';

				if( strlen( value ) + strlen( linebuf ) > HBUFSIZ-1 ) {
					fprintf( stderr, "%s: multi-line text too long (max=%d).\n", parm, HBUFSIZ); 
					gdp_exit(); 
					}
				else sprintf( value, "%s\n%s", value, linebuf );
				}
			ufound = 1;
			break;
			}
		}
	}
	
/* if still not found, or if type checking turned on, check master template */
if( !ufound || Do_cons_check ) {  
	
	/* get proc name */
	sscanf( Chunk[0], "%*s %s", proc ); 
	proc[ strlen( proc )-1 ] = '\0';

	/* find where proc starts in storage */
	if( strcmp( proc, lastproc )!= 0 ) {
		strcpy( lastproc, proc );
		index = 0;
		for( k = 0; ; k++ ) {
			if( k > 0 && Tdx[k] == 0 ) { fprintf( stderr, "%s: Proc has no template.\n", proc ); gdp_exit(); }
			index += Tdx[k];
			sscanf( Template[ index ], "%*s %s", p );
			p[ strlen( p ) -1 ] = '\0';
			if( strcmp( proc, p )==0 ) {
				start = index;
				stop = start + Tdx[k+1];
				start ++;  /* adjust to skip over Proc line */
				break;
				}
			}
		}
	
	mfound = 0;
	for( k = start; k < stop; k++ ) {
		sscanf( Template[k], "%s", p );
		if( p[0] == '#' || p[0] == ':' ) continue;
		if( !is_parm_name( p )) continue;
		p[ strlen( p ) -1 ] = '\0'; /* get rid of colon */
		if( strcmp( parm, p )==0 ) {
			ix = 0;
			getfld( linebuf, Template[k], &ix ); /* get first field-- not needed */
			getfld( constraint, Template[k], &ix );
			if( !ufound ) {
				getfld( linebuf, Template[k], &ix ); /* get value */

				strcpy( value, linebuf );
				}
	
			/* get additional lines, if any */
			while( k < stop ) {
				k++;
				strcpy( linebuf, Template[k] );
				strip_ws( linebuf );
				if( linebuf[0] == ':' || linebuf[0] == '#' ) continue;
				if( linebuf[0] == '$' ) cmdline_subst( linebuf );
				sscanf( linebuf, "%s", p );
				if( is_parm_name( p )) break;
				if( strlen( value ) + strlen( linebuf ) > HBUFSIZ-1 ) {
					fprintf( stderr, "%s: multi-line text too long (max=%d).\n", parm, HBUFSIZ ); 
					gdp_exit(); 
					}
				else sprintf( value, "%s\n%s", value, linebuf );
				}
			mfound = 1;
			break;
			}
		}
	
	if( mfound ) {
		if( ! constraint_check( value, constraint ) ) { 
			fprintf( stderr, "parm %s Line %d in proc %s.\n", parm, inln, proc ); 
			gdp_exit(); 
			}
		strcpy( Last_constraint, constraint );
		}
	else	{
		fprintf( stderr, "Parameter '%s' not found. Line %d in proc %s.\n", parm, inln, proc ); 
		gdp_exit();
		}
	
	}
/* always returns here (except error exit) */
strip_ws( value );

/* trap for debugging @/
* if( strcmp( parm, "Subtitle.above" )==0 ) {
*	fprintf( stderr, "$%s$", value );
*	for( i = 0; i < Clines; i++ ) fprintf( stderr, "%s", Chunk[i] );
*	fprintf( stderr, "###\n" );
*	for( i = 0; i < Ilines; i++ ) fprintf( stderr, "%s", Ichunk[i] );
*	fprintf( stderr, "&&&\n" );
*	}
*/

return( 1 );
}




/* ====================================== */
/* changes val to command line argument if value is  (, , etc) */
cmdline_subst( val )
char val[];
{
if( val[0] == '$' && atoi( &val[1] ) > 0 ) { 
	if( atoi( &val[1] ) > Argc-1 ) { fprintf( stderr, "%s: not that many arguments\n", val ); gdp_exit(); }
	strcpy( val, Arg[ atoi( &val[1] ) ] );
	}
}
/* ===================================== */
/* returns true if s is in parameter name format
   or if "Proc"
*/
is_parm_name( s )
char s[];
{
int l;
l = strlen( s );
if( isupper( s[0] ) && s[ l-1 ] == ':' && s[ l-2 ] != '\134' ) return( 1 );
else if( strcmp( s, "Proc" )==0 ) return( 1 );
else return( 0 );
}
