#include "ipl.x"
#define DATA 0
#define ABSOLUTE 1

Text( )
{
int sys, ln, embedded, p, ix;
double x, y, theta, size;
char s1[40], s2[40];
FILE *fp;

gget( Buf, "System" );
if( Buf[0] == 'd' ) { 
	sys = DATA; 
	if( DXlo == 0 && DXhi == 0 ) { fprintf( stderr, "No graphics area.\n" ); gdp_exit(); }
	}
else sys = ABSOLUTE;

gget( Buf, "Font" ); NTfont( Buf );
gget( Buf, "Size" ); size = atof( Buf ); NTptsize( size );
gget( Buf, "Position" ); 
sscanf( Buf, "%lf %lf", &x, &y );
gget( Buf, "Direction" ); theta = atof( Buf );

if( theta != 0 ) NTchardir( theta );

gget( Buf, "Embeddedcoords" );
if( Buf[0] == 'y' ) embedded = 1; else embedded = 0;

gget( Buf, "File" );
if( strlen( Buf ) > 0 ) fp = fopen( Buf, "r" );		/* text in a file */
else 	{		/* text given-- put it in a file */
	gget( Buf, "Text" );
	text_tofile( Buf, Tempfile );
	fp = fopen( Tempfile, "r" );
	}
if( fp == NULL ) { fprintf( stderr, "Cant open text file" );  gdp_exit(); }

/* read the file and print the text */
if( !embedded ) {
	if( sys == DATA ) NTm( x, y );
	else NTmov( x, y );
	}

ln = 1;
while( fgets( Buf, 200, fp ) != NULL ) {
	if( embedded ) {
		ix = 0;
		strcpy( s1, getok( Buf, &ix ) );
		strcpy( s2, getok( Buf, &ix ) );
		if( goodnum( s1, &p ) && goodnum( s2, &p ) ) {
			x = atof( s1 ); y = atof( s2 );
			if( sys == DATA ) NTm( x, y );
			else NTmov( x, y );
			strcpy( Buf, &Buf[ix] );
			strip_ws( Buf );
			}
		}
	NTtext( Buf );
	if( sys == DATA ) NTmov( da_x(x), da_y(y)-(ln*Chh) ); 
	else NTmov( x, y-(ln*Chh) );
	ln++;
	}

fclose( fp );
NTchardir( 0 );
}
