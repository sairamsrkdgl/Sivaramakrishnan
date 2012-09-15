/* legend() - creates legend for line and bar graphs */
#include "ipl.x"
#define SHADE	1
#define MARK	4	
#define LINE	8	
#define MAXENTRIES 10

Legend( )
{
double	val[MAXENTRIES],
	locx, locy,
	margin,
	piclen,
	lablen,
	depth,
	x, ylo, yhi,
	lx, ly, cx, cy, bxw, ty,
	mrksize,
	stdsize,
	pm[MAXENTRIES],
	ms[MAXENTRIES],
	th[MAXENTRIES],
	thick = 1,
	magnify;
int 	i, 
	n,
	format,
	lenmax, 
	outline,
	nms,
	nlt,
	npm,
	nth,
	nmf,
	nent;
char 	ent[MAXENTRIES][100],
	poscode[4],
	stdfont[FONTNAME_LEN],
	mrkfont[FONTNAME_LEN],
	lt[MAXENTRIES][3],
	mrk[MAXENTRIES][10],
	mf[MAXENTRIES][20],
	linetype[3];



gget( Buf, "Entry.font" );
strcpy( stdfont, Buf ); NTfont( Buf ); /* go to standard font */

strcpy( mrkfont, Stdfont ); /* default mark font */

/* position of legend can either be specified using a corner A,B,C,D or by giving an
   x,y location for the upper left corner of the legend.
*/
gget( poscode, "Corner" );
gget( Buf, "Location" ); 
if( strlen( Buf ) > 0 ) sscanf( Buf, "%lf %lf", &locx, &locy );
else { locx = 0; locy = 0; }

gget( Buf, "Location.system" );
if( strcmp( Buf, "data" )==0 ) { locx = da_x( locx ); locy = da_y( locy ); }



/* get entries and find longest one */
gget( Buf, "Entry" );
lenmax = 0;
nent = countln( Buf );
if( nent > MAXENTRIES ) { fprintf( stderr, "Maximum of 10 legend entries" ); nent = 10; }
getln( "" );
for( i = 0; i < nent; i++ ) {
	strcpy( ent[i], getln( Buf ) );
	if( strlen( ent[i] ) > lenmax ) lenmax = strlen( ent[i] ); 
	}

/* figure if we're doing lines or shades, and get 'em */
format = 0;
gget( Buf, "Shade" ); 
if( strlen( Buf ) > 0 ) { 
	format = SHADE; 
	n = sscanf( Buf, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf", 
		&val[0],&val[1],&val[2],&val[3],&val[4],&val[5],&val[6],&val[7],&val[8],&val[9] );
	if( n < nent ) { fprintf( stderr, "need %d shades.\n", nent ); gdp_exit(); }
	}
gget( Buf, "Linetype" );
if( strlen( Buf ) > 0 ) {
	format = LINE;
	nlt = sscanf( Buf, "%s %s %s %s %s %s %s %s %s %s", 
		lt[0],lt[1],lt[2],lt[3],lt[4],lt[5],lt[6],lt[7],lt[8],lt[9] );
	gget( Buf, "Linetype.magnify" );
	npm = sscanf( Buf, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf", 
		&pm[0],&pm[1],&pm[2],&pm[3],&pm[4],&pm[5],&pm[6],&pm[7],&pm[8],&pm[9] );
	gget( Buf, "Linethick" );
	nth = sscanf( Buf, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf", 
		&th[0],&th[1],&th[2],&th[3],&th[4],&th[5],&th[6],&th[7],&th[8],&th[9] );
	}


gget( Buf, "Mark" );
if( strlen( Buf ) > 0 ) {
	format += MARK;
	n = sscanf( Buf, "%s %s %s %s %s %s %s %s %s %s", 
		mrk[0],mrk[1],mrk[2],mrk[3],mrk[4],mrk[5],mrk[6],mrk[7],mrk[8],mrk[9] );
	if( n < nent ) { fprintf( stderr, "need %d marks\n", nent ); gdp_exit(); }

	gget( Buf, "Mark.font" );
	nmf = sscanf( Buf, "%s %s %s %s %s %s %s %s %s %s",
		mf[0], mf[1], mf[2], mf[3], mf[4], mf[5], mf[6], mf[7], mf[8], mf[9] );

	gget( Buf, "Mark.size" ); 
	nms = sscanf( Buf, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf", 
		&ms[0],&ms[1],&ms[2], &ms[3], &ms[4], &ms[5], &ms[6], &ms[7], &ms[8], &ms[9] );
	}


margin = .20;
lablen = lenmax * (Chh*0.6); /* lenmax = space in plot units of longest label */

if( format == SHADE || format == MARK ) piclen = .20;  /* piclen = size in x of line or swatch */
else piclen = 1.10;

depth = (Chh*1.2) * (nent+1);
if( format == SHADE ) depth *= 1.65;

/* generate legend */
if( locx != 0 || locy != 0 ) { lx = locx; ly = locy; }
else if( poscode[0] == 'A' ) { lx = Xlo + 0.1; ly = Yhi - 0.1; }
else if( poscode[0] == 'B' ) { lx = Xhi - (lablen+piclen+margin); ly = Yhi - 0.1; }
else if( poscode[0] == 'C' ) { lx = Xhi - (lablen+piclen+margin); ly = Ylo + depth; }
else if( poscode[0] == 'D' ) { lx = Xlo + 0.1; ly = Ylo + depth ; }
else { fprintf( stderr, "Legend location must be given. Use either Corner or Location parameter." ); gdp_exit(); }

/* do a legend title */
gget( Buf, "Title.size" );
NTptsize( atof( Buf ) );
gget( Buf, "Title" );
ty = ly + (countln( Buf ) * Chh);
getln( "" );
for( i = 0; i < countln( Buf ); i++ ) {
	NTmov( lx, ty );
	NTcentext( getln( Buf ), (lablen+piclen) );
	ty -= Chh;
	}

gget( Buf, "Entry.size" ); stdsize = atof( Buf ); NTptsize( stdsize );

gget( Buf, "Backshade" );
if( strlen( Buf ) > 0 ) {
	ab_rect( lx-.08, ly - (depth)+0.02, lx+(lablen+piclen), ly+.08, atof( Buf ), 0 );
	}
gget( Buf, "Outlinebars" );
if( Buf[0] == 'y' ) outline = YES;
else outline = NO;

cx = lx; cy = ly;
for( i = 0; i < nent; i++ ) {

	if( format >= MARK && i < nms ) mrksize = ms[i];
	if( format >= LINE && i < nlt ) strcpy( linetype, lt[i] );
	if( format >= LINE && i < npm ) magnify = pm[i];
	if( format >= LINE && i < nth ) thick = th[i];
	if( format >= MARK && i < nmf ) strcpy( mrkfont, mf[i] );

	/* trap */ if( format >= LINE && ( thick < 0.01 || thick > 20 )) { 
		fprintf( stderr, "Internal error. Try again.\n" ); gdp_exit(); 
		}

	if( format == SHADE || format == MARK ) {
		if( format == SHADE ) {
			cy -= 0.1; x = cx; bxw =  0.2; ylo = cy - 0.1; yhi = cy + 0.1;
			if( outline || val[i] == 1.0 )ab_rect( x, ylo, x+bxw, yhi, val[i], 1 );
			else ab_rect( x, ylo, x+bxw, yhi, val[i], 0 );
			}
		else if( format == MARK ) { 
			NTfont( mrkfont ); NTptsize( mrksize );
			if( strncmp( mrk[i], "sym", 3 )==0 ) point( cx, cy, mrk[i], Chh*0.4 );
			else {
				NTmov( cx, cy-0.06 ); 
				NTtext( mrk[i] ); 
				NTfont( stdfont ); 
				}
			NTmov( cx, cy ); 
			NTfont( stdfont ); NTptsize( stdsize );
			}
		NTmov( cx+0.4, cy-0.06 );
		NTtext( ent[i] );
		}
	else if( format >= LINE ) {
		NTlinetype( linetype, thick, magnify );
		NTmov( cx, cy );
		NTlin( cx+1.0, cy );
		if( format == LINE + MARK ) {
			NTfont( mrkfont ); NTptsize( mrksize );
			if( strncmp( mrk[i], "sym", 3 )==0 ) {
				point( cx+0.2, cy, mrk[i], Chh*0.4 );
				point( cx+0.8, cy, mrk[i], Chh*0.4 );
				}
			else {
				NTfont( mrkfont ); NTptsize( mrksize ); 
				NTmov( cx+0.2, cy-0.03 ); NTtext( mrk[i] );
				NTmov( cx+0.8, cy-0.03 ); NTtext( mrk[i] ); 
				NTfont( stdfont ); NTptsize( stdsize );
				}
			NTfont( stdfont ); NTptsize( stdsize );
			}
		NTmov( cx+1.1, cy-0.06 );
		NTtext( ent[i] );
		NTnormline();
		}
	cy -= (Chh*1.2); 
	}
}
