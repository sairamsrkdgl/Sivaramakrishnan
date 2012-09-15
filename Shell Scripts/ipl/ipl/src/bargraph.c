/* horizontal (standard) bar graphs */
#include "ipl.x"
#define STACK	0
#define CLUSTER 1


Bargraph( )
{
int 	k[8], 
	idf[8],
	format,
	label,
	n,
	nc,
	i, j, jj,
	p,
	xfld,
	xset,
	start,
	outline;

double s[8],
	accum,
	zer,
	curx,
	xspace,
	subspace,
	y,
	y2,
	f,
	lblpos,
	sep,
	msep = 0.03;
char str[10];


/* get the data field list */
gget( Buf, "Field" ); 
n = 0;
while( n < 1 ) { 
	n = sscanf( Buf, "%d %d %d %d %d %d %d %d", &k[0], &k[1], &k[2], &k[3], &k[4], &k[5], &k[6], &k[7] );
	if( N_d_fields == 1 ) strcpy( Buf, "1" );
	else if( n < 1 ) { fprintf( stderr, "Field parameter(s) for bar graph are missing.\n" ); gdp_exit(); }
	}
for( i = 0; i < n; i++ ) 
	if( k[i] < 1 || k[i] > N_d_fields ) { fprintf( stderr, "Field is bad\n" ); gdp_exit(); }

/* get the label field list, if any */
gget( Buf, "Idfield" ); 
if( strlen( Buf ) > 0 ) { 
	label = YES;
	sscanf( Buf, "%d %d %d %d %d %d %d %d", &idf[0], &idf[1], &idf[2], &idf[3], &idf[4], &idf[5], &idf[6], &idf[7] );
	for( i = 0; i < n; i++ ) 
		if( idf[i] < 1 || idf[i] > N_d_fields ) { fprintf( stderr, "Idfield is bad\n" ); gdp_exit(); }
	}
else label = NO;

gget( Buf, "Format" );  
if( strcmp( Buf, "stack" )==0 ) format = STACK;
else format = CLUSTER;

DXtic = 1.0;

/* get bar shades */
gget( Buf, "Shade" ); 
sscanf( Buf, "%lf %lf %lf %lf %lf %lf %lf %lf", &s[0], &s[1], &s[2], &s[3], &s[4], &s[5], &s[6], &s[7] );

/* get zero line */
gget( Buf, "Zeroat" ); 
if( goodnum( Buf, &p )) zer = atof( Buf );
else zer = DYlo;

/* get label size */
if( label ) { gget( Buf, "Idfield.size" ); NTptsize( atof( Buf ) ) };

/* distance of label from bar top */
if( label ) { gget( Buf, "Idfield.position" ); lblpos = atof( Buf ); }

/* outline or not */
gget( Buf, "Outlinebars" ); if( Buf[0] == 'y' ) outline = YES; else outline = NO;

/* x distance between major bar spaces */
gget( Buf, "Separation" ); sep = atof( Buf );

gget( Buf, "Separation.sub" ); msep = atof( Buf );

xspace = ( (Xhi-Xlo)/((DXhi-DXlo)+1) ) - (sep*Scale_x);

if( format == CLUSTER ) nc = n; else nc = 1;

gget( Buf, "Killwild" ); /* option for aborting plot if any values out of range */
if( atof( Buf ) != 0 ) {
	for( i = 1; i <= N_d_rows; i++ ) {
		if( atof( D[i-1][ k[0] -1 ] ) > atof( Buf ) ) {
			fprintf( stderr, "Note: This Bargraph terminated due to a value of %s.\n", D[i-1][k[0]-1] );
			return( 0 );
			}
		}
	}

gget( Buf, "Xfield" ); /* allow placement of bars by a data field */
if( strlen( Buf ) > 0 ) {
	xset = 1;
	xfld = atoi( Buf );
	if( xfld < 1 || xfld > N_d_fields ) { fprintf( stderr, "Yfield bad.\n" ); gdp_exit(); }
	}
else xset = 0;

gget( Buf, "Xstart.0or1" ); /* allow starting at 0 or 1 */
if( Buf[0] == '0' ) { start = 0; }
else { start = 1; }

for( i = 1; i <= N_d_rows; i++ ) {
	if( xset ) curx = da_x( atof( D[i-1][xfld-1] ) ) - (xspace/2);
	else curx = da_x((double)(DXlo+i+(start-1))) - (xspace/2);

	subspace = ( xspace / nc );
	for( j = 0; j < nc; j++ ) {
		if( !goodnum( D[i-1][ k[j]-1 ], &p )) {
			fprintf( stderr, "Warning, row %d, field %d, is bad (%s)\n", i, k[j], D[i-1][ k[j]-1] );
			curx += subspace;
			continue;
			}
		y = atof(D[i-1][ k[j]-1 ]);
		if( y != DYlo )
			ab_rect( curx, da_y(zer), curx+(subspace-msep), da_y(y), s[j], (s[j]==1)?1:outline );
		if( label ) {
			if( y < zer || format == STACK ) f = (-lblpos)-Chh; else f = lblpos;
			strcpy( str, D[i-1][ idf[j]-1 ] );
			NTmov( curx, da_y(y)+f );
			NTcentext( str, subspace-msep );
			}
		if( format == STACK ) for( jj = 1; jj < n; jj++ ) {
			if( !goodnum( D[i-1][ k[jj] -1 ], &p ) ) {
				fprintf( stderr, "Warning, row %d, field %d, is bad (%s)\n", i, k[jj], D[i-1][k[jj]-1] );
				continue;
				}
			y2 = y + atof( D[i-1][ k[jj] -1 ] );
			if( y2 != DYlo )
				ab_rect( curx, da_y(y), curx+(subspace-msep), da_y(y2), s[jj], (s[jj]==1)?1:outline );
			if( label ) {
				if( y2 < zer || format == STACK ) f = (-lblpos)-Chh; else f = lblpos;
				NTmov( curx, da_y(y2)+f );
				strcpy( str, D[i-1][ idf[jj]-1 ] );
				NTcentext( str, subspace-msep );
				}
			y = y2;
			}

		curx += subspace;
		}
	}

gget( Buf, "Segment" );
if( Buf[0] == 'y' ) 
	for( f = DYlo+DYtic; f < DYhi; f += DYtic ) 
		rect( DXlo + 0.2, f-(DYhi*0.003), DXhi - 0.2, f+(DYhi*0.004), 1.0, 0 );
}
