/* Postscript driver for IPL.	
   (c) 1989 Steve Grubb
*/

#include <stdio.h>
#define YES 1
#define NO 0
#define PORTRAIT 0
#define LANDSCAPE 1
#define MARG_X 14 
#define MARG_Y 8 
#define PAGWIDTH 600;

static int orient;		/* paper orientation */
static int orgx, orgy;		/* location of origin on page */
static int theta;		/* current rotation for page orientation */
static char font[50];		/* current font name */
static int chdir;	 	/* char direction in degrees */
static int curpointsz;		/* current char size in points */
char *getok();
double atof();

/* ============================= */
PSsetup( )    /* initialize */
{  
/* set globals */
orient = -1;
strcpy( font, "/Helvetica" );
theta = 0;
chdir = 0;
curpointsz = 10;

/* print header */
printf(  
"%%!PS-Adobe-1.0\n%%%%Creator: IPL V1.0(c) 1989 Steve Grubb\n%%%%Pages: (atend)\n" );

	
/* set up macros (mainly to reduce output verbosity) */
printf(  "/sset\n" );
printf(  "{ translate rotate } def \n" );
printf(  "/sclr\n" );
printf(  "{ rotate translate } def \n" );
printf(  "/mv { moveto } def\n" );
printf(  "/np { newpath } def\n" );
printf(  "/ln { lineto } def\n" );
printf(  "/st { stroke } def\n" );
printf(  "/sh { show } def\n" );
printf(  "/cent { stringwidth pop sub 2 div 0 rmoveto } def\n" );
printf(  "/rjust { stringwidth pop sub 0 rmoveto } def\n" );

/* load default font */
printf(  "%s findfont\n", font );
printf(  "%d scalefont setfont\n", (int) curpointsz );

/* set up standard line width */
printf(  "1 setlinewidth\n" );

printf(  "%%%%EndProlog\n%%%%Page: ? 1\nsave\n" ); /* begin first page */
}


/* ============================= */
PSmoveto( x, y )
double x, y;
{

/* convert to p.s. units (1/72 inch) */
x = ( x * 72.0 ) +MARG_X; y = ( y * 72.0 ) + MARG_Y; 
printf(  "np\n%-5.2f %-5.2f mv\n", x, y ); 
}


/* ============================= */
PSlineto( x, y )
double x, y;
{
/* convert to p.s. units */
x = ( x * 72 ) +MARG_X; 
y = ( y * 72 ) +MARG_Y; 
printf(  "%-5.2f %-5.2f ln\n", x, y );
}

/* ============================== */
PSstroke( )
{
printf( "st\n" );
}


/* ============================= */
PSpath( x, y )
double x, y;
{
/* convert to p.s. units */
x = ( x * 72 ) +MARG_X; y = ( y * 72 ) + MARG_Y; 
printf(  "%-5.2f %-5.2f ln\n",  x, y );
}


/* ============================= */
PSshade( s )
double s;
{
printf(  "closepath\n%3.2f setgray\nfill\n0 setgray\n", s );
}

/* ============================== */
PSpaper( i )
int i;
{
if( orient != -1 ) return( 1 );		/* paper orientation - can only be done once per page */
if( i == 1 ) { /* handle landscape mode-- it's put into portrait mode before beginning each page */
	orgx = PAGWIDTH; 
	orgy = 0; 
	theta = 90; 
	printf(  "%d %d %d sset\n", theta, orgx, orgy );
	} 
orient = (int) i;
}


/* ================================= */
PStext( com, x, y, s, w )
char com;
double x, y;
char s[];
double w;
{
char str[400];
int i, j, k;

x = (x*72)+MARG_X;  y = (y*72)+MARG_Y; w *= 72;

/* if text direction is not normal do a rotate then move.. */
if( chdir != 0 ) printf(  "%d %-5.2f %-5.2f sset 0 0 mv\n", chdir, x, y ); 
/* normal direction-- do a move.. */
else printf(  "%-5.2f %-5.2f mv ", x, y );

if( member( com, "CJ" )) strip_ws( s );

/* escape out parens */
for( i = 0, j = 0; i < strlen( s ); i++ ) {
	if( s[i] == '(' || s[i] == ')' ) { str[j++] = '\\'; str[j++] = s[i]; }
	else str[j++] = s[i];
	}
str[j] = '\0';

/* centered text */
if( com == 'C' ) printf(  "%-5.2f (%s) cent ", w, str ); 
else if( com == 'J' ) printf(  "%-5.2f (%s) rjust ", w, str );

/* do the string */
printf(  "(%s) sh\n", str );

if( chdir != 0 ) printf(  "%-5.2f %-5.2f %d sclr\n", -x, -y, -chdir ); /* restore */
}


/* ================================= */
PSpointsize( p )
int p;
{
if( p != curpointsz ) { 	/* char size (specified in points) */
	curpointsz = p;
	printf(  "%s findfont\n", font );
	printf(  "%d scalefont\nsetfont\n", p );
	}
}


/* ================================== */
PSfont( f )
char f[];
{
if( strcmp( f, font ) != 0 ) {
	strcpy( font, f );
	printf(  "%s findfont\n", font );
	printf(  "%d scalefont setfont\n", curpointsz );
	}
}

/* ================================== */
PSchardir( t )
int t;
{
chdir = t;
}


/* ================================== */
PSlinetype( s, x, y )
char s[];
double x, y;
{
/* X = line width;  Y = dash pattern magnification (0.1 to 10)
 *  S indicates dash pattern.  If S is "0", an unbroken (normal) line is produced.
 *  If S is "1" through "8", a preset dash pattern is used.  Otherwise, S is
 *  assumed to hold the dash pattern string "[ n1 n2 n3.. ]".	
 */
static int dash[10][6]= { {0,0,0,0,0,0}, {1,1}, {3,1}, {5,1}, {2,1,1,1}, {4,1,1,1}, {6,1,1,1}, 
			  {2,1,1,1,1,1}, {4,1,1,1,1,1}, {6,1,1,1,1,1} };
int ltype, i;

printf(  "%3.1f setlinewidth\n", x );
if( strlen( s ) < 1 || strcmp( s, "0" )==0 ) printf(  "[] 0 setdash\n" );
else 	{
	if( strlen( s ) > 1 ) { 
		ltype = 0; 
		sscanf( s, "%d %d %d %d %d %d", &dash[0][0], &dash[0][1], &dash[0][2], 
			&dash[0][3], &dash[0][4], &dash[0][5] );
		}
	else ltype = atoi( s );
	printf(  "[" );
	for( i = 0; i < 6; i++ ) 
		if( dash[ ltype ][ i ] > 0 ) printf(  "%3.1f ", dash[ ltype ][ i ] * y );
	printf(  "] 0 setdash\n" );
	}
}
	

/* =================================== */

PSshow()
{
if( orient == 1 )printf(  "%d %d %d sclr\n", -orgx, -orgy, -theta ); /* restore rotation */
orient = -1; 
printf( "showpage\nrestore\nsave\n" );
}

/* =================================== */
