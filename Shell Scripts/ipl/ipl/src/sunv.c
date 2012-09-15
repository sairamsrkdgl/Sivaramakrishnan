/* sunview.c
IPL sunview driver. (c) 1989 Steve Grubb.

Use compile option -DNOEVENTS if you do not need to handle mouse/keyboard events.
Otherwise, you have to write a function handle_event( x, y, e ) that will get
called whenever waiting for an event.  Arguments: x and y are double values indicating
the position of the mouse at the time of the event, in your units;  e is the integer
event identifier (see Sunview Pixwind guide), generally the ascii value of keys, and
other values for mouse events.

User origin is lower-left.


*/

#include <stdio.h>
#include <ctype.h>
#include <sys/file.h>
#include <suntool/sunview.h>  /* sun stuff */
#include <suntool/canvas.h>  /* sun stuff */
#include "shading.h"
#define SUNV_FONT_PATH "/usr/lib/fonts/fixedwidthfonts"
#define MAX_D_ROWS 1000
#define AREA_WIDTH 900
#define AREA_HEIGHT 900

#define WHITE 0
#define BLACK 255

double xscale, yscale;
double xsca_inv(), ysca_inv();
double oldx, oldy;

int color, backcolor;

/* polygon vertexes */
struct pr_pos vlist[MAX_D_ROWS];
int nv = 0;

/* sunview variables.. */
Frame w_fr;
Canvas w_cvs;
int w_width, w_height;
struct rect w_rect; /* drawing area rect */
struct pixwin *w_pw; /* window handle */
Pixfont *w_font;  /* current font */
int w_ppc;	/* pixels per character (across) for current font */
Pr_brush w_br, *w_brush = &w_br ; /* line width */
Pr_texture w_tex, *w_texture = &w_tex; /* line dash pattern */

static void SVevent();
int w_waitflag = 0;

short w_dash[4][10][6]= {
		{ {1}, {1,1}, {3,1}, {5,1}, {2,1,1,1}, {4,1,1,1}, {6,1,1,1}, 
		  {2,1,1,1,1,1}, {4,1,1,1,1,1}, {6,1,1,1,1,1} },
		{ {2}, {2,2}, {6,2}, {10,2}, {4,2,2,2}, {8,2,2,2}, {12,2,2,2}, 
		  {4,2,2,2,2,2}, {8,2,2,2,2,2}, {12,2,2,2,2,2} },
		{ {3}, {3,3}, {9,3}, {15,3}, {6,3,3,3}, {12,3,3,3}, {18,3,3,3}, 
		  {6,3,3,3,3,3}, {12,3,3,3,3,3}, {18,3,3,3,3,3} },
		{ {4}, {4,4}, {12,4}, {20,4}, {8,4,4,4}, {16,4,4,4}, {24,4,4,4}, 
		  {8,4,4,4,4,4}, {16,4,4,4,4,4}, {24,4,4,4,4,4} }
			};
/* icon */
static short ic_image[258] = {
#include "ipl.icon"
};
mpr_static(gfxic_mpr, 64, 64, 1, ic_image);

static	struct icon icon = {64, 64, (struct pixrect *)NULL, 0, 0, 64, 64,
	    &gfxic_mpr, 0, 0, 0, 0, NULL, (struct pixfont *)NULL,
	    ICON_BKGRDCLR};


/* =============================================== */
/* SVsetup()-- initialize the window and parameters */
SVsetup( x_max, y_max )
double x_max, y_max;
{
w_fr = window_create( NULL, FRAME, 
	WIN_HEIGHT, AREA_HEIGHT, WIN_WIDTH, AREA_WIDTH, WIN_X, 0, WIN_Y, 0, 
	FRAME_LABEL, "sipl", FRAME_ICON, &icon, 0 );
w_cvs = window_create( w_fr, CANVAS, WIN_EVENT_PROC, SVevent, 0 );
window_set( w_cvs, WIN_CONSUME_KBD_EVENT, WIN_ASCII_EVENTS, 0 );
w_pw = canvas_pixwin( w_cvs );

color = BLACK;
backcolor = WHITE;
w_height = AREA_HEIGHT - 20;	/* in pixels */
w_width = AREA_WIDTH;		/* in pixels */
xscale = (double)(w_width) / (x_max);
yscale = (double)(w_height) / (y_max);
w_texture->pattern = w_dash[0][0];
w_brush->width = 1; 
window_set( w_fr, WIN_SHOW, TRUE, 0 );
(void) notify_dispatch();
}

/* ====================== */
static void SVevent( ecvs, e )
Canvas ecvs;
Event *e;
{
int x1, y1, eid;

#ifndef NOEVENT
x1 = event_x( e );
y1 = event_y( e );
if( event_is_ascii(e) ) {
	w_waitflag = handle_event( xsca_inv( x1 ), ysca_inv( y1 ), event_action( e ) );
	}
if( event_is_button(e) ) {
	/* use generic button identifiers.. */
	if( event_action(e)  == MS_LEFT ) eid = 1001;
	if( event_action(e) == MS_MIDDLE ) eid = 1002;
	if( event_action(e) == MS_RIGHT ) eid = 1003;
	w_waitflag = handle_event( xsca_inv( x1 ), ysca_inv( y1 ), eid );
	}
#endif
}

/* ====================== */
SVwait()
{
w_waitflag = 0;
#ifndef NOEVENT
while( ! w_waitflag ) {
	(void) notify_dispatch();
	usleep( 10000 );  /* check notifier every .01 seconds */
	}
#endif
}

/* ==================== scale in x to window size */
xsca( f )
double f;
{ 
int iout;
iout = (int) (f * xscale); 
return( iout );
}

/* ==================== inverse of xsca */
double xsca_inv( i )
int i;
{
double out;
out = (double) i / xscale;
return( out );
}

/* ==================== scale in y to window size */
ysca( f )
double f;
{ 
int iout;
iout = w_height - (int) (f * yscale);
return( iout );
}

/* ===================== inverse of ysca */
double ysca_inv( i )
int i;
{
double out;
out = (double)(w_height-i) / yscale;
return( out );
}

/* ==================== clear the display */
SVclear()
{
pw_writebackground( w_pw, 0, 0, w_width, w_height, (PIX_SRC | PIX_COLOR(backcolor)) );
}

/* ====================  line to */
SVlineto(x,y)  
double x, y;
{ 
int a, b, c, d;
a = xsca( oldx ); b = ysca( oldy ); c = xsca( x ); d = ysca( y );
pw_line( w_pw, a, b, c, d, w_brush, w_texture, (PIX_SRC | PIX_COLOR(color)) );
oldx=x;
oldy=y;
}

/* =====================  moveto */
SVmoveto(x,y)   
double x, y;
{
oldx = x;
oldy = y;
}

/* ===================== dot */
SVdot(x,y)    
double x, y;
{
pw_put( w_pw, xsca(x), ysca(y), color );
}

/* ===================== read pixel value */
SVread(x,y)   
double x, y;
{
pw_get( w_pw, xsca(x), ysca(y) );
}

/* ===================== set point size of type */
/* SVpointsize */
SVpointsize( p )
int p;
{
char fontdir[60], buf[65];
strcpy( fontdir, SUNV_FONT_PATH );

if( p <= 6 ) 		{ p = 6; w_ppc = 4; }
if( p == 7 || p == 8 ) 	{ p = 7; w_ppc = 6; }
if( p == 9 || p == 10 )	{ p = 10; w_ppc = 7; }
if( p == 11 )		{ w_ppc = 7; }
if( p == 12 || p == 13 ){ p = 12; w_ppc = 8; }
if( p == 14 || p == 15 ){ p = 14; w_ppc = 9; }
if( p == 16 || p == 17 ){ p = 16; w_ppc = 10; }
if( p >= 18 && p <= 22 ){ p = 18; w_ppc = 11; }
if( p > 22 ){ p = 24; w_ppc = 14; }

if( p < 10 ) sprintf( buf, "%s/screen.r.%d", fontdir, p );
else if( p == 11 ) sprintf( buf, "%s/screen.r.%d", fontdir, p );
else sprintf( buf, "%s/cour.b.%d", fontdir, p );

if( w_font != (Pixfont *) 0 ) pf_close( w_font );
w_font = pf_open( buf );
if( w_font == NULL ) { w_font = pw_pfsysopen(); }
return( w_ppc );
}

/* ==================== display left adjusted text starting at current position */
SVtext( s, aw )
char s[];
double *aw; /* actual string width in inches (sent back) */
{
pw_ttext( w_pw, xsca(oldx), ysca(oldy), (PIX_SRC | PIX_COLOR(color)), w_font, s );
oldx+= (double)((strlen(s)-1)* w_ppc ); 
*aw = xsca_inv( (strlen(s)-1) * w_ppc );
}

/* ==================== display centered text */
SVcentext( s, w, x, aw )
char s[];
double w;
double *x; /* actual X starting point in inches (sent back) */
double *aw; /* actual string width in inches (sent back) */
{
double width;

strip_ws( s );
width = xsca_inv( w_ppc*strlen( s ) );
SVmoveto( oldx+((w-width)/2.0), oldy );
SVtext( s, aw );
*x = xsca_inv((int)( oldx+((w-width)/2.0) ) );
}

/* ==================== display right-justified text */
SVrightjust( s, w, x, aw )
char s[];
double w;
double *x; /* actual X starting point in inches (sent back) */
double *aw; /* actual string width in inches (sent back) */
{
double width;

strip_ws( s );
width = xsca_inv( w_ppc*strlen( s ) );
SVmoveto( oldx+(w-width), oldy );
SVtext( s, aw );
*x = xsca_inv((int)(oldx+(w-width) ));
}

/* ==================== set the drawing color */
SVcolor( d, b )
int d, b;
{
color = d; backcolor = b;
}


/* ==================== add to "fill path" */
SVpath( x, y )
double x, y;
{
if( nv == 0 ) { vlist[0].x = xsca( oldx ); vlist[0].y = ysca( oldy ); nv++; }
vlist[nv].x = xsca(x);
vlist[nv].y = ysca(y);
nv++;
}

/* ==================== fill prev. defined polygon path with color c */
SVshade( c )
double c; 
{
int n[1];
Pixrect *pr;
if( c >= 0.0 && c <= 1.0 ) {
	if( c == 1.0 ) pr = &w_s1_0;
	else if( c > .95 ) pr = &w_s_90;
	else if( c > .90 ) pr = &w_s_80;
	else if( c > .85 ) pr = &w_s_70;
	else if( c > .75 ) pr = &w_s_60;
	else if( c > .65 ) pr = &w_s_50;
	else if( c > .55 ) pr = &w_s_40;
	else if( c > .45 ) pr = &w_s_30;
	else if( c > .35 ) pr = &w_s_20;
	else if( c > .15 ) pr = &w_s_10;
	else if( c >= 0.0 ) pr = &w_s_00;
	else pr = (Pixrect *) 0;
	}

n[0] = nv;
pw_polygon_2( w_pw, 0, 0, 1, n, vlist, (PIX_SRC | PIX_COLOR((int)(c))), pr, 0, 0 ); 
nv = 0;
}

/* ========= set line dotting attribs ============= */

SVlinetype( t, w, mag )
char t[];
double w, mag;
{
int i, j, k;

if( strlen( t ) < 1 ) return( 0 );
if( mag < 1 ) mag = 1;
if( mag > 4 ) mag = 4;
w_brush->width = (int)((w*1.6)+0.5);
w_texture->pattern = w_dash[ (int)mag-1 ][ atoi( t ) % 10 ];
}

/* ========= batch calls for efficiency  ========== */
SVbatch_on()
{
(void) notify_dispatch();
pw_batch_on( w_pw );
}
SVbatch_off()
{
(void) notify_dispatch();
pw_batch_off( w_pw );
}



/* =========================================== */
/* sets up color map, 0 = white, 255 = black, color mixes from 1 to 114 */
setup_colormap()
{
u_char red[256], grn[256], blu[256];
int i, j;

red[0] = (u_char) 0;				/* background color */
grn[0] = (u_char) 0;
blu[0] = (u_char) 0;

	for (i=0; i<19; i++) {
		red[i +   1] = (u_char) ( 0.99 * 256 );		/* ramp to yellow */
		grn[i +   1] = (u_char) ( 0.055*i * 256 );
		blu[i +   1] = (u_char) ( 0.0 * 256 );

		red[i +  20] = (u_char) ( (0.99-0.055*i) * 256 );	/* ramp to green */
		grn[i +  20] = (u_char) ( 0.99 * 256 );
		blu[i +  20] = (u_char) ( 0.0 * 256 );

		red[i +  39] = (u_char) ( 0.0 * 256 );		/* ramp to turqouise */
		grn[i +  39] = (u_char) ( 0.99 * 256 );
		blu[i +  39] = (u_char) ( 0.055*i * 256 );

		red[i +  58] = (u_char) ( 0.0 * 256 );		/* ramp to blue */
		grn[i +  58] = (u_char) ( ( 0.99-0.055*i ) * 256 );
		blu[i +  58] = (u_char) ( 0.99 * 256 );

		red[i +  77] = (u_char) ( 0.055*i * 256 );		/* ramp to violet */
		grn[i +  77] = (u_char) ( 0.0 * 256 );
		blu[i +  77] = (u_char) ( 0.99 * 256 );

		red[i +  96] = (u_char) ( 0.99 * 256 );		/* ramp to red */
		grn[i +  96] = (u_char) ( 0.0 * 256 );
		blu[i +  96] = (u_char) ( (0.99-0.055*i) * 256 );
		}
for( i = 115, j = 0; i < 125; i++, j+= 25 ) red[i] = blu[i] = grn[i] = j;
for( i = 125, j = 255; i < 256; i++ ) { j = abs(j-255); red[i] = blu[i] = grn[i] = j ; }


pw_setcmsname( w_pw, "dots" );
pw_putcolormap( w_pw, 0, 256, red, grn, blu );
}


