#include "ipl.x"
/* draw a data point */
/* point styles are selected by the code string "symNSf.ff", where N is an integer
   0-9 selecting the shape, S is a lower-case character selecting the style, and
   f.ff is an optional shade, which overrides that selected as part of the style.  
   N and S are required.  See point1.g in the examples.
*/
#define NVARIAT 18
#define NSHAPE 6

Point( )
{
double x, y, r;
char code[20];

gget( Buf, "Mark" );
strcpy( code, Buf );
gget( Buf, "Position" );
sscanf( Buf, "%lf %lf", &x, &y );
gget( Buf, "Size" );
sscanf( Buf, "%lf", &r );
NTptsize( r );
r = Chh * 0.4;
point( x, y, code, r );
}


point( x, y, code, r )
double x, y; 	/* point location in abs space */
char code[];	/* pre-set symbol name */
double r; 	/* radius of dot in absolute units */
{
int i;
double g, theta;
static char prevcode[10] = "";
static double prev_r, shade;
static int inc, ins;
static int nc[] =    {  3,  3,  4,  4,  5, 12 };	/* number of corners */
static int nt[] =    { 90,270,  0, 45, 90, 90, 90 };    /* location (in deg) to start building point */
/* preset outline thicknesses.. */
static double ol[] = { .5,  1,1.5,  2, .5, .5, .5, .5, .5,  0,  0,  0,  0, 0, -0.5, -1, -1.5, -2 }; 
/* preset shades..  */
static double sh[] = { -1, -1, -1, -1,  1, .9, .7, .5, .3, .9, .7, .5, .3, 0, -1, -1,  -1,  -1 }; 
static double h[14][2]; /* the offsets */

/* no-op code */
if( strcmp( code, "sym00" ) == 0 ) return( 0 );

if( strcmp( code, prevcode ) != 0 || r != prev_r ) {
	strcpy( prevcode, code );
	prev_r = r;
	inc = ((code[3] - '0') -1 ) % NSHAPE;
	if( code[4] == '\0' ) code[4] = 'a';
	ins = (code[4] - 'a') % NVARIAT;
	if( strlen( code ) > 5 )shade = atof( &code[5] );
	else shade = sh[ins];

	theta = 360.0 / (double)nc[inc];
	/* get offsets */
	g = nt[inc];
	for( i = 0; i < nc[inc]; i++ ) {
		h[i][0] = r * cos( (g*3.1415927)/180.0 );
		h[i][1] = r * sin( (g*3.1415927)/180.0 );
		g += theta;
		}
	}

/* lock-on */
/* shade point */
if( shade >= 0 ) {
	NTmov( x+h[0][0], y+h[0][1] );
	for( i = 1; i < nc[inc]; i++ ) NTpath( x+h[i][0], y+h[i][1] );
	NTshade( shade );
	}

/* draw perimeter point */
if( ol[ins] > 0.0 ) {
	NTlinetype( "0", ol[ins], 1.0 );
	NTmov( x+h[0][0], y+h[0][1] );
	for( i = 1; i < nc[inc]; i++ ) NTlin( x+h[i][0], y+h[i][1] );
	NTlin( x+h[0][0], y+h[0][1] );
	NTnormline();
	}

/* draw spokes */
if( ol[ins] < 0.0 ) {
	double fabs();
	NTlinetype( "0", fabs(ol[ins]), 1.0 );
	for( i = 0; i < nc[inc]; i++ ) { NTmov( x, y ); NTlin( x+h[i][0], y+h[i][1] ); }
	NTnormline();
	}
/* lock-off */

}
