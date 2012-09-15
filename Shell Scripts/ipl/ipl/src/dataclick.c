/* This allows coordinate data to be entered using the mouse. */

#include "ipl.x"

dataclick( fnm )
char fnm[];
{

double x, y;
int s;
FILE *fp;

if( strlen( fnm ) > 0 ) {
	fp = fopen( fnm, "w" );
	if( fp == NULL ) { fprintf( stderr, "Can't open file.\n" ); return( 0 ); }
	}

message( "", "Point and click with", "left mouse button.", "Press right button to quit." );
while( 1 ) {
	get_event( &x, &y, &s );
	if( s == MOUSE_RIGHT ) break;
	if( s == MOUSE_LEFT ) { 
		sprintf( Buf, "%-9.3f %-9.3f ", ab_x( x ), ab_y( y ) ); 
		message( Buf, "", "(Data coordinates)", "Press right button to quit." );
		if( strlen( fnm ) > 0 ) fprintf( fp, "%s\n", Buf );
		point( x, y, "sym6a", 0.08 );
		get_event( &x, &y, &s ); /* get spurious bounce-back (???) */
		}

	}
if( strlen( fnm ) > 0 ) fclose( fp );
}
