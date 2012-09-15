#include <stdio.h>

/* ============================ */
/* IPL device interface (c) 1989 Steve Grubb */
/* This must be compiled either with or without flag NOSUNVIEW */

static char pdev;
static int vertchar = 0;

pcode( op, x, y, s )
char op; /* op code */
double x, y;  /* coordinates */
char s[];     /* optional character string */
{
int outx, outy, move;
char ans[10], pbuf[400];
static double curx = 0, cury = 0;
static int doclip = 0;
double p1[2], p2[2];
double x1, y1, x2, y2;
double width, realx, chh;
static int new = 0, drawing = 0;



/* interface to postscript driver */
if( pdev == 't' ) {    

	if( op != 'L' ) { 
		if( drawing ) PSstroke(); 
		drawing = 0;
		}

	if( op == 'L' ) { 
		if( new ) PSmoveto( curx, cury ); 
		PSlineto( x, y );
		new = 0;
		drawing = 1;
		}
	else if( op == 'M' ) { new = 1; curx = x; cury = y; }
	else if( op == 'P' ) { 
		if( new ) PSmoveto( curx, cury ); 
		PSpath( x, y ); 
		new = 0;
		}
	else if( op == 'T' ) PStext( op, curx, cury, s, 0.0 );
	else if( op == 'C' ) PStext( op, curx, cury, s, x );
	else if( op == 'J' ) PStext( op, curx, cury, s, x );
	else if( op == 'S' ) PSshade( x );
	else if( op == 'O' ) PSpaper( (int)x );
	else if( op == 'I' ) PSpointsize( (int)x );
	else if( op == 'F' ) PSfont( s );
	else if( op == 'D' ) PSchardir( (int)x );
	else if( op == 'Y' ) PSlinetype( s, x, y );
	else if( op == 'Z' ) PSshow();
	else if( op == 'K' ) doclip = 1;
	else if( op == 'k' ) doclip = 0;
	}


/* interface to sunview driver */
/* the routines SVtext, SVcentext, and SVrightjust return information
   for keeping a bounding box list. */
#ifndef NOSUNVIEW
else if( pdev == 's' ) { 
	if( op == 'L' ) SVlineto( x, y );
	else if( op == 'P' ) SVpath( x, y );
	else if( op == 'M' ) SVmoveto( x, y );
	else if( op == 'T' && !vertchar ) SVtext( s, &width );
	else if( op == 'B' ) SVbatch_on();
	else if( op == 'b' ) SVbatch_off();
	else if( op == 'Y' ) SVlinetype( s, x, y ); 
	else if( op == 'S' ) SVshade( x );
	else if( op == 'I' || op == 'F' ) { SVpointsize( (int)(x) ); chh = (x+2)/72.0; }
	else if( op == 'C' && !vertchar ) SVcentext( s, x, &curx, &width );
	else if( op == 'J' && !vertchar ) SVrightjust( s, x, &curx, &width );
	else if( op == 'W' ) SVwait(); 
	else if( op == 'D' ) {
		if( x == 90 || x == 270 ) vertchar = 1;
		else vertchar = 0;
		}
	else if( op == 'Z' ) { 
		SVclear();
		}
	else if( op == 'K' ) doclip = 1;
	else if( op == 'k' ) doclip = 0;


	}
#endif

else if( pdev == 'm' ) return( 1 );  /* term-oriented composer-- no graphics */
else { fprintf( stderr, "pcode: %c: no such device\n", pdev ); exit(); }



}


/* ========================================================== */
/* initialize */
NTinit( dev )
char dev;
{


int yr, mon, day, hr, min, sec, i;
char host[30]; 

sysdate( &mon, &day, &yr ); systime( &hr, &min, &sec );

fprintf( stderr, "IPL graphics system version 1.0 by Steve Grubb\n" );

/* initialize devices */
if( dev == 's' || dev == 'v' ) {  
	SVsetup( 11.0, 11.0 );
	SVclear();
	pdev = 's';
	}

else if( dev == 't' ) {
	PSsetup( );
	pdev = 't';
	}

else if( dev == 'm' ) { pdev = 'm'; return( 1 ); }

else { fprintf( stderr, "NTinit: %c: no such device\n", dev ); exit(); }
}
