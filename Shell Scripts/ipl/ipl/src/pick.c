/* These routines are for getting user input.  If Dev is 'v' (indicating
   sunview composer), window and mouse-oriented input is used.  If Dev is
   'm' (indicating terminal composer), terminal oriented input is used.
*/

#include "ipl.x"
#define MAXBB 80

/* event modes */
#define PICK 1		/* getting a menu selection */
#define STRING 2	/* getting a \n terminated string */
#define EVENTS 3	/* getting any mouse-button or keyboard event */

static FILE *fp = NULL;
static double bb[MAXBB][4];
static double bbxlo, bbylo, bbxhi, bbyhi;
static char name[MAXBB][80];
static char selection[80];
static int nbb, 
       menu_mode,
       event_mode;
static double evx, evy;
static int eid;
static int getting_text;
static double Y;
int box_given = 0;
int transparent = 0;
double BX1, BY1, BX2, BY2;


/* ======================================== */
/* getpick - draws a menu then gets user response and
   sends back the name of the menu box selected.  Menu
   can be defined in a file or by a string of tokens.
   New is normally 1, but can be set to 0 to append to
   an existing bounding box list.  Draw is normally 1
   but can be set to 0 to "overlay" graphics.  
*/
getpick( rtn, proc, new, draw )
char rtn[]; /* user's selection placed here */
char proc[]; 	/* name of menu def file */
int new;	/* if yes, start a new bounding box list */
int draw;	/* if yes, draw menu; if no, don't draw menu */

{
int ix, fromstring, ixx, e;
char coords[80], label[80], buf[200];
double x, y;


fromstring = 0;
if( strlen( proc ) < 1 ) goto SKIP; /* for null argument.. */

sprintf( buf, "%s/%s", Templatepath, proc );
fp = fopen( buf, "r" );
if( fp == NULL ) {
	fromstring = 1; 
	y = 10; 
	ixx = 0; 
	}

if( new ) {
	nbb = 0;
	bbxlo = 999; bbxhi = -999; bbylo = 999; bbyhi = -999;
	}

if( draw ) {
	NTbatch_on();
	NTfont( "/Helvetica-Oblique" );
	NTptsize( 10 );
	NTlinetype( "0", 3, 1 );
	}
if( Dev == 'm' ) fprintf( stderr, "=====================\n" );
while( 1 ) { 
	if( fromstring ) {
		bb[nbb][0] = 8.0; bb[nbb][1] = y-0.4; bb[nbb][2] = 11.0; bb[nbb][3] = y;
		strcpy( name[nbb], getok( proc, &ixx ));
		if( strlen( name[nbb] ) < 1 ) break;
		strcpy( label, name[nbb] );
		y -= 0.5;
		}
	else	{  /* from file */
		if( fgets( buf, 200, fp ) == NULL ) break;
		ix = 0;
		getfld( coords, buf, &ix );
		getfld( label, buf, &ix );
		getfld( name[nbb], buf, &ix );
		sscanf( coords, "%lf %lf %lf %lf", &bb[nbb][0], &bb[nbb][1], &bb[nbb][2], &bb[nbb][3] );
		}
	
	if( draw ) {
		ab_rect( bb[nbb][0], bb[nbb][1], bb[nbb][2], bb[nbb][3], 1.0, 1 );
		NTmov( bb[nbb][0], bb[nbb][1] + ((bb[nbb][3]-bb[nbb][1])/2.0) );
		NTcentext( label, bb[nbb][2]-bb[nbb][0] );
		}

	if( bb[nbb][0] < bbxlo ) bbxlo = bb[nbb][0];
	if( bb[nbb][1] < bbylo ) bbylo = bb[nbb][1];
	if( bb[nbb][2] > bbxhi ) bbxhi = bb[nbb][2];
	if( bb[nbb][3] > bbyhi ) bbyhi = bb[nbb][3];

	if( Dev == 'm' ) fprintf( stderr, "%-2d) %s\n", nbb+1, name[nbb] ); /* term menu */ 
	nbb ++;
	}

if( fp != NULL ) fclose( fp );

if( draw ) {
	NTnormline();
	NTbatch_off();
	}
	
SKIP:
event_mode = PICK;
NTwait();  /* let user pick */
get_event( &x, &y, &e ); /* spurious event */

if( Dev == 'm' ) while( 1 ) {                 /* term menu */
	fprintf( stderr, "Selection: " );
	fgets( buf, 10, stdin ); 
	if( atoi( buf ) > 0 && atoi( buf ) <= nbb )
		{ strcpy( selection, name[ atoi( buf ) -1 ] ); break; }
	}
	
strcpy( rtn, selection );
}


/* ================================ */

/* This gets called by the driver when 1) we are waiting for an event by calling NTwait(),
   and 2) a key or mouse event happens. */
/* NOTE:  menu picking is in effect while getting a string */
handle_event( x, y, e )
double x, y;
int e;
{
int i;

/* for menus.. */
if( event_mode == PICK ) {
	/* look up (x,y) in bounding box list */
	if( x > bbxlo && x < bbxhi && y > bbylo && y < bbyhi ) {
		for( i = nbb-1; i >= 0; i-- ) {
			if( x > bb[i][0] && y > bb[i][1] && x < bb[i][2] && y < bb[i][3] ) break;
			}
		if( i == nbb ) return( 0 ); /* not found */
		strcpy( selection, name[i] ); return( 1 );  /* found */
		}
	}
/* for strings */
else if( event_mode == STRING && e > 0 && e < 128  ) {
	selection[0] = (char) e;
	selection[1] = '\0';
	return( 1 );
	}
else if( event_mode == EVENTS ) {
	evx = x; evy = y; eid = e;
	return( 1 );
	}
return( 0 );
}


				
/* ================================== */

message( s1, s2, s3, s4  )
char s1[], s2[], s3[], s4[];
{
NTptsize( 10 );
ab_rect( 8.0, 10.0, 11.0, 11.0, 1.0, 1 );
NTmov( 8.1, 10.7 ); NTtext( s1 );
NTmov( 8.1, 10.5 ); NTtext( s2 );
NTmov( 8.1, 10.3 ); NTtext( s3 );
NTmov( 8.1, 10.1 ); NTtext( s4 );
if( Dev == 'm' ) fprintf( stderr, "\t\t* %s\n\t\t* %s\n\t\t* %s\n\t\t* %s\n", s1, s2, s3, s4 );
}


/* ==================================== */

get_event( x, y, e )
double *x, *y;
int *e;
{
char ans[20];
	
event_mode = EVENTS;
NTwait();
*x = evx; *y = evy; *e = eid;
}


/* ==================================== */
/* prompts user for one line of text */


get_string( s, p )
char s[], p[];
{
int i;
double x, y;

if( Dev == 'm' ) { /* terminal interface */
	fprintf( stderr, "%s: ", p );
	fgets( s, 100, stdin );
	return( 1 );
	}

if( !box_given ) { BX1 = 0.3; BY1 = 0.1; BX2 = 9; BY2 = 0.5; }

if( getting_text ) y = Y; /* Y set in get_text() */
else y = BY1;
x = BX1 + 0.2 + ( strlen( p ) * Chh * 0.5 );

/* do box and prompt */
NTlinetype( "0", 3, 1 );

if( getting_text && !transparent ) ab_rect( BX1, y-0.1, BX2, y+Chh, 1.0, 0 );
else if( !transparent )ab_rect( BX1, y-0.1, BX2, y+0.4, 1.0, 1 );

NTnormline();
NTmov( BX1+0.1, y );
NTtext( p );
NTmov( x, y );	

i = 0;
while( 1 ) {
	NTmov( x, y );
	event_mode = STRING;
	NTwait(); /* let user enter char */
	if( selection[0] == '\b' || selection[0] == '\177' ) { 
		if( i == 0 ) continue;
		i--;
		ab_rect( (x-(Chh*0.5)), y-(Chh*0.28), x, (y+(Chh*0.85)), 1.0, 0 ); 
		x -= ( Chh * 0.5 ); 
		continue; 
		}
	else if( selection[0] == '\015' || selection[0] == '\012' ) break;
	else if( selection[0] == '\004' ) {  /* control-D stops */
		strcpy( s, "\004" ); 
		return( 1 ); 
		}
	else NTtext( selection );

	s[i++] = selection[0];
	x += (Chh * 0.5 );
	}
s[i] = '\0';
if( !getting_text && !transparent )ab_rect( BX1, y-0.1, BX2, y+0.4, 0.98, 0 );
}

/* ========================================= */
/* gets multi-row text. */
get_text( buf, p )
char buf[], p[];
{
int i, j, maxlen;
double x;
char uin[20];

if( Dev == 'm' ) { /* for terminal interface */
	fprintf( stderr, "=======================\n" );
	sprintf( buf, "%s  Enter \".\" to stop..\n", p );
	fprintf( stderr, "%s", buf );
	i = 0;
	while( 1 ) {
		fgets( &buf[i], 100, stdin );
		if( strcmp( &buf[i], ".\n" )==0 ) break; 
		i += strlen( &buf[i] );
		}
	buf[ i-1 ] = '\0';
	return( 1 );
	}


sprintf( buf, "%s  Press Control-D to stop..\n", p );

maxlen = 80; /* max length of any line */

if( !box_given ) { BX1 = 0.3; BY1 = 0.3; BX2 = 9; BY2 = 8.0; }

x = BX1;
Y = BY2;

if( !transparent )ab_rect( x-0.1, BY1, BX2+0.1, Y+0.2, 1.0, 1 );
NTmov( x, Y );
if( !transparent) { NTcentext( buf, BX2-BX1 ); Y -= Chh; }

getting_text = 1;
nbb = 0;
/* control D quits */
for( i = 0; i < MAXBB; i++ ) {
	bb[i][0] = BX1; bb[i][1] = Y-(Chh*1.6);  bb[i][2] = BX2; bb[i][3] = Y; sprintf( name[i], "%d", i );
	get_string( &(buf[i*maxlen]), "" );
	if( buf[i*maxlen] == '\004' ) break;
	for( j = (i*maxlen)+ strlen( &buf[i*maxlen] ); j < ((i+1)*maxlen)-1; j++ ) buf[j] = ' ';
	buf[ ((i+1)*maxlen) -1 ] = '\n';
	Y -= Chh;
	}
buf[i*maxlen] = '\0';
bbxlo = 0; bbxhi = 11; bbylo = 0; bbyhi = 11;

getting_text = 0;
}


/* =============================== */
get_string_box( s, p, x1, y1, x2, y2 )
char s[], p[];
double x1, y1, x2, y2;
{
box_given = 1;
BX1 = x1; BY1 = y1; BX2 = x2; BY2 = y2;
get_string( s, p );
box_given = 0;
}


/* =============================== */
get_text_box( s, p, x1, y1, x2, y2 )
char s[], p[];
double x1, y1, x2, y2;
{
box_given = 1;
BX1 = x1; BY1 = y1; BX2 = x2; BY2 = y2;
get_text( s, p );
box_given = 0;
}

/* =============================== */
get_text_box_tp( s, p, x1, y1, x2, y2 )
char s[], p[];
double x1, y1, x2, y2;
{
transparent = 1;
get_text_box( s, p, x1, y1, x2, y2 );
transparent = 0;
}



