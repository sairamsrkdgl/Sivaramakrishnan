/* This allows coordinate data to be entered using the mouse. */

#include "ipl.x"

get_point( x, y )
double *x, *y;
{

int s;
double x2, y2;
char ans[20];

if( Dev == 'm' ) { /* terminal interface */
	fprintf( stderr, "Enter a position in inches.  First x: " );
	fgets( ans, 20, stdin ); *x = atof( ans );
	fprintf( stderr, "Now y: " ); fgets( ans, 20, stdin ); *y = atof( ans );
	return( 1 );
	}

get_event( x, y, &s );
point( *x, *y, "sym6a", 0.08 );
get_event( &x2, &y2, &s ); /* get spurious bounce-back (???) */

}
