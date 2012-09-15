/* draw - for drawing with lines */
#include "ipl.x"
#define ABSOLUTE 0
#define DATA 1

Draw( )
{
int sys, n, i, p;
double mag, thick, x, y, x2, y2;
char ltype[10];

gget( Buf, "System" );
if( strcmp( Buf, "absolute" )==0 ) sys = ABSOLUTE;
else 	{
	sys = DATA;
	if( DXlo == 0 && DXhi == 0 ) { fprintf( stderr, "No graphics area.\n" ); gdp_exit(); }
	}

/* get line style parameters */
gget( Buf, "Linetype" ); strcpy( ltype, Buf );

gget( Buf, "Linetype.magnify" ); 
if( goodnum( Buf, &p )) mag = atof( Buf );
else mag = 1;

gget( Buf, "Linethick" ); thick = atof( Buf );


/* set line style */
NTlinetype( ltype, thick, mag );

/* get points */
gget( Buf, "Points" );
getln( "" );
for( i = 0; i < countln( Buf ); i++ ) {
	n = sscanf( getln( Buf ), "%lf %lf %lf %lf", &x, &y, &x2, &y2 );
	if( sys == DATA && n == 2 ) NTl( x, y );
	else if( sys == DATA && n == 2 && i == 0 ) NTm( x, y );
	else if( sys == DATA && n == 4 ) { NTm( x, y ); NTl( x2, y2 ); }
	else if( sys == ABSOLUTE && n == 2 ) NTlin( x, y );
	else if( sys == ABSOLUTE && n == 2 && i == 0 ) NTmov( x, y );
	else if( sys == ABSOLUTE && n == 4 ) { NTmov( x, y ); NTlin( x2, y2 ); }
	else { fprintf( stderr, "Points should contain either one or two coord pairs per line.\n" ); gdp_exit(); }
	}

NTnormline(); /* return line to normal */
}	
