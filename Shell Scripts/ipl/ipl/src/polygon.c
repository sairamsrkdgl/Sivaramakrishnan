/* polygon - for creating polygons, optionally shaded and outlined */
#include "ipl.x"
#define ABSOLUTE 0
#define DATA 1

Polygon( )
{
int sys, n, i, p, outline;
double mag, thick, x, y, fx, fy;
char ltype[10], s1[12], s2[12], s3[12], s4[12];

gget( Buf, "System" );
if( strcmp( Buf, "absolute" )==0 ) sys = ABSOLUTE;
else 	{
	sys = DATA;
	if( DXlo == 0 && DXhi == 0 ) { fprintf( stderr, "No graphics area.\n" ); gdp_exit(); }
	}

gget( Buf, "Outline" );
if( Buf[0] == 'y' ) outline = 1;
else outline = 0;

/* get line style parameters */
gget( Buf, "Linetype" ); strcpy( ltype, Buf );

gget( Buf, "Linetype.magnify" ); 
if( goodnum( Buf, &p )) mag = atof( Buf );
else mag = 1;

gget( Buf, "Linethick" ); thick = atof( Buf );


/* set line style */
NTlinetype( ltype, thick, mag );

/* get points */
gget( Buf2, "Rectangle" );
if( strlen( Buf2 ) > 0 ) {
	sscanf( Buf2, "%s %s %s %s", s1, s2, s3, s4 );
	sprintf( Buf, "%s %s\n%s %s\n%s %s\n%s %s", s1, s2, s1, s4, s3, s4, s3, s2 );
	}
else gget( Buf, "Points" );
if( strlen( Buf ) < 1 ) { fprintf( stderr, "Points or Rectangle not specified.\n" ); gdp_exit(); }
getln( "" );
for( i = 0; i < countln( Buf ); i++ ) {
	n = sscanf( getln( Buf ), "%lf %lf", &x, &y );
	if( sys == DATA && i == 0 ) NTm( x, y ); 
	else if( sys == ABSOLUTE && i == 0 ) NTmov( x, y );
	else if( sys == DATA ) NTp( x, y ); 
	else if( sys == ABSOLUTE ) NTpath( x, y );
	else { fprintf( stderr, "Points should contain one coord pair per line.\n" ); gdp_exit(); }
	}
gget( Buf2, "Shade" );
if( strlen( Buf2 ) > 0 ) NTshade( atof( Buf2 ) );

if( outline ) {
	getln( "" );  /* reset getln() */
	for( i = 0; i < countln( Buf ); i++ ) {
		n = sscanf( getln( Buf ), "%lf %lf", &x, &y );
		if( sys == DATA && i == 0 ) { NTm( x, y ); fx = x; fy = y; }
		else if( sys == ABSOLUTE && i == 0 ) { NTmov( x, y ); fx = x; fy = y; }
		else if( sys == DATA ) NTl( x, y ); 
		else if( sys == ABSOLUTE ) NTlin( x, y );
		else { fprintf( stderr, "Points should contain one coord pair per line.\n" ); gdp_exit(); }
		}
	if( sys == DATA ) NTl( fx, fy ); 
	else if( sys == ABSOLUTE ) NTlin( fx, fy );
	}


NTnormline(); /* return line to normal */
}	
