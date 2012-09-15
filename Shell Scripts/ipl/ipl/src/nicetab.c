/* Nicetab() - takes an ascii table an sets it up in a proportional font */
#include <ctype.h>
#include "ipl.x"
#define MAX_ROW	 132
#define MAX_COL	 132

#define LINE 0
#define TWO_BLANK 1

#define LEFT 0
#define CENTER 1
#define RIGHT 2
#define ROW 3

extern char *Areacoords[];
extern int Nareas;


Nicetab( )
{

int i, iln, j, k, r, c, nrows, n;	/* counters, row, col */
int isnum;			/* true if token is a number */
int p;				/* position of decimal point in number */
int nblank;			/* blank line counter */
int first;			/* first time thru */
int more;			/* more pages to do? */
int maxc;			/* max number of chars on any one line */
int coord[4];			/* returned by getphrase(), 
					holds col position of string left, center, and right, and row */
char t[MAX_COL][MAX_ROW];	/* text matrix */
int shadeblocks;		/* table row shading option */
int shade;			/* shade flag */
char area[60];			/* paper area */
FILE *fp;


Scale_discipline_x = LINEAR;	/* either LINEAR or LOG */
Scale_discipline_y = LINEAR;	/* either LINEAR or LOG */
/* get file containing ascii table, running it through expand to change tabs to spaces */
gget( area, "File" );
if( area[0] == '-' ) { 
	area[0] = '\0'; 
	}
sprintf( Buf, "expand %s > %s", area, Tempfile );
system( Buf );
fp = fopen( Tempfile, "r" ); 
if( fp == NULL ) { fprintf( stderr, "Can't open %s.", Tempfile ); gdp_exit(); }

/* read ascii file into t array, converting tabs to spaces */
more = 0; r = 0; maxc = 0;
/* initialize storage */
for( i = 0; i < 132; i++ ) {
	for( j = 0; j < 132; j++ ) t[i][j] = ' ';
	t[i][131] = '\0';
	}
while( fgets( Buf, 512, fp ) != NULL ) {
	c = 0;
	if( strlen( Buf ) >= MAX_COL || r >= MAX_ROW ) 
		fprintf( stderr, "Sorry, ascii table size limit %d rows x %d cols", MAX_ROW, MAX_COL );
	for( i = 0; i < strlen( Buf ); i++ ) {
		if( Buf[i] == '\t' ) for( ; c % 8 != 0; c++ ) t[ r ][ c ] = ' ';/* handle tabs */
		else t[ r ][ c++ ] = Buf[ i ];
		}
	if( c > maxc ) maxc = c;
	r++;
	}
fclose( fp );
Ymax = r;
nrows = r;

/* set up an area (supercedes areadef because table dimensions aren't known 
   ahead of time) */
gget( area, "Area.rectangle" );
if( strlen( area ) > 0 ) {
	n = sscanf( area, "%lf %lf %lf %lf", &Xlo, &Ylo, &Xhi, &Yhi );
	}
else	{
	gget( area, "Area" );
	for( i = 0; i < Nareas; i++ ) if( strncmp( area, Areacoords[i], strlen( area )) ==0 ) break;
	if( i == Nareas ) { fprintf( stderr, "Can't find preset area '%s'.\n", area ); gdp_exit(); }
	if( Paper == 0 ) /* portrait */
		n = sscanf( Areacoords[i], "%*s %lf %lf %lf %lf", &Xlo, &Ylo, &Xhi, &Yhi );
	else n = sscanf( Areacoords[i], "%*s %*s %*s %*s %*s %lf %lf %lf %lf", &Xlo, &Ylo, &Xhi, &Yhi );
	if( n != 4 ) { fprintf( stderr, "Error in area presets.\n" ); gdp_exit(); }
	}

/* set scaling based on longest line and number of rows */
setscale_x( Xlo, Xhi, 0.0, (double) maxc );
setscale_y( Ylo, Yhi, 0.0, Ymax );

/* title assumed to be everything until at least 2 blank lines encountered */
gget( Buf, "Title.font" ); 
NTfont( Buf );
gget( Buf, "Title.size" ); 
NTptsize( atof(Buf) );
gget( Buf, "Body.only" );
if( Buf[0] == 'y' ) { r = 0; goto BODY; }

nblank = -999;
for( r = 0; r < nrows ; r++ ) {
	/* count blank lines */
	if( sscanf( t[r], "%s", Buf ) < 1 ) {
		nblank++;
		if( nblank >= 2 ) break;
		else continue;
		}
	else nblank = 0;
	getphrase( Buf, t[r], coord, &c, LINE );
	coord[ROW] = r;
	center( Buf, coord ); 
	}
if( r > 15 ) fprintf( stderr, "Warning: use at least two blank lines to separate table title from body.\n" );


/* do BODY of table */
BODY:
gget( Buf, "Body.font" ); 
NTfont( Buf );
gget( Buf, "Body.size" ); 
NTptsize( atof(Buf) );

shadeblocks = NO;
shade = NO;
for( ; r < nrows; r++ ) {
	c = 0;

	/* special operators */
	if( t[r][0] == '@' ) {
		if( strncmp( t[r], "@shadeblocks-on", 15 )==0 ) { shadeblocks = YES; iln = -1; }
		else if( strncmp( t[r], "@shadeblocks-off", 16 )==0 ) { shadeblocks = NO; shade = NO; }
		else if( strncmp( t[r], "@shade-on", 9 )==0 ) shade = YES;
		else if( strncmp( t[r], "@shade-off", 10 )==0 ) shade = NO; 

		continue;
		}
			
	if( shadeblocks ) {
		iln++;
		if( ( iln / 3 ) % 2 == 0 ) shade = YES;
		else shade = NO;
		}
	
	if( shade ) rect( 0.0,(double)((Ymax-r)-.15), (double)(maxc),(double)((Ymax-r)+.85), 0.98, NO );

			
	if( sscanf( t[r], "%s", Buf ) < 1 ) continue; /* don't mess with blank lines */

	first = 1;
	while( 1 ) {

		/* get a phrase, and position of beginning and end.. */
		getphrase( Buf, t[r], coord, &c, TWO_BLANK, first );
		coord[ROW] = r;
		if( Buf[0] == '\0' ) break;


		/* see if its a number */
		isnum = goodnum( Buf, &p );

		/* horizontal rulings (entire token must be -,_,or =) */
		for( i = 0; i < strlen( Buf ); i++ ) if( !member( Buf[i], "-_=" )) break;

		if( strlen( Buf ) > 2 && i == strlen( Buf )) ruler( Buf, coord );

		/* vertical rulings */
		else if( Buf[0] == '|' ) vert_ruler( coord, nrows, t );

		/* if phrase is 1st thing on line (within 8 chars), left justify */
		else if( first && coord[LEFT] < 10 ) ljust( Buf, coord );

		/* if phrase is alpha and longer than 2 chars and not near the top, left justify (?) */
		else if( strlen( Buf ) > 2 && isalpha( Buf[0] ) && r > 10 ) ljust( Buf, coord );
		
		/* numbers.. right-just */ 
		else if( isnum ) rjust( Buf, coord );

		/* everything else gets centered */
		else center( Buf, coord );

		first = 0;
		}
	}
}


/* ====================================== */
getphrase( out, line, coord, c, mode, first )
char out[], 	/* returned phrase */
     line[];	/* input line */
int coord[4];	/* returned location */
int *c;		/* position in line */
int mode;	/* tells if phrases are 
			1) == LINE everything on a line; 
			2) == TWO_BLANK delimited by 2 spaces or 1 tab 
*/
int first;	/* behaves differently if getting 1st phrase on a line. */
{
int i;

if( mode == LINE ) {	/* working with whole line */
	if( sscanf( line, "%s", out ) < 1 ) { out[0] = '\0'; return( 0 ); } /* blank line */
	for( i = 0; ; i++ ) if( !member( line[i], "\t \n" ) ) break; /* skip white space on left */
	for( (*c) = strlen( line ); ; (*c)-- ) if( !member( line[i], "\t \n" ) ) break; /* skip white space on right */
	}

else if( mode == TWO_BLANK ) {  
	if( sscanf( &line[ (*c) ], "%s", out ) < 1 ) { out[0] = '\0'; return( 0 ); }
	for( i = (*c); i < strlen( line ) ; i++ ) 
		if( !member( line[i], "\t \n" ) ) break; /* skip white space on left */
	/* go until we hit a stopping rule */
	for( (*c) = i; (*c)<strlen( line ) ; (*c)++ ) {
		if( line[ (*c) ] == ' ' && line[ (*c)-1 ] == ' ' ) break;
		else if( first ) continue;
		else if( member( line[ (*c) ], "()[]<>|" ) && (*c)==i ) { (*c)++; break; } /* first token-- return it */
		else if( member( line[ (*c) ], "()[]<>|\t" ) ) break; /* not first token, return and get it next time */
		}
	}

strncpy( out, &line[i], (*c)-i );
for( ; ; (*c)-- ) if( ! member( out[ ((*c)-i)-1 ], " \t\n" ) ) break;
out[ (*c)-i ] = '\0';

coord[ LEFT ] = i; coord[ CENTER ] = ((*c) + i ) / 2;  coord[ RIGHT ] = (*c);
	
}
/* ====================================== */
center( str, coord )
char str[];
int coord[4];
{
NTm( coord[LEFT], Ymax-coord[ROW] );
NTcentext( str, da_x( (double)coord[RIGHT] )-da_x( (double)coord[LEFT] ) );
}
/* ====================================== */
center_float( str, coord )
char str[];
int coord[4];
{

}
/* ====================================== */
ljust( str, coord )
char str[];
int coord[4];
{
NTm( coord[LEFT], Ymax-coord[ROW] );
NTtext( str );
}
/* ====================================== */
rjust( str, coord )
char str[];
int coord[4];
{
NTm( coord[LEFT], Ymax-coord[ROW] );
NTrightjust( str, da_x( (double)coord[RIGHT] ) - da_x( (double) coord[LEFT]) );
}
/* ================================== */
ruler( str, coord )
char str[];
int coord[4];
{ 
NTm( coord[LEFT], Ymax-coord[ROW]+.5 );
NTl( coord[RIGHT], Ymax-coord[ROW]+.5 );
}

/* ================================= */
vert_ruler( coord, nrows, t )
int coord[4], nrows;
char t[][MAX_ROW];
{
int i;
NTm( coord[LEFT]+.5, (Ymax-coord[ROW])+1 );
for( i = coord[ROW]+1; i < nrows; i++ ) 
	if( t[i][ coord[LEFT] ] == '|' ) { NTl( coord[LEFT]+.5, (Ymax-i) ); return( 1 ); }
}
