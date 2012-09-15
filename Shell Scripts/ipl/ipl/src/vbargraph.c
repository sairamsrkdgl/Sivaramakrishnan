#include "ipl.x"
#define STACK	0
#define CLUSTER 1


Vbargraph( )
{
int 	k[8], 
	idf[8],
	format,
	label,
	n,
	nc,
	i, j, jj,
	p,
	start,
	yset,
	yfld,
	outline;

double s[8],
	accum,
	zer,
	cury,
	yspace,
	subspace,
	x,
	x2,
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
	else if( n < 1 ) { fprintf( stderr, "Field bad\n" ); gdp_exit(); }
	}
for( i = 0; i < n; i++ ) 
	if( k[i] < 1 || k[i] > N_d_fields ) { fprintf( stderr, "Field out of range" ); gdp_exit(); }

/* get the label field list, if any */
gget( Buf, "Idfield" ); 
if( strlen( Buf ) > 0 ) { 
	label = YES;
	sscanf( Buf, "%d %d %d %d %d %d %d %d", &idf[0], &idf[1], &idf[2], &idf[3], &idf[4], &idf[5], &idf[6], &idf[7] );
	for( i = 0; i < n; i++ ) 
		if( idf[i] < 1 || idf[i] > N_d_fields ) { fprintf( stderr, "Idfield bad.\n" ); gdp_exit(); }
	}
else label = NO;


gget( Buf, "Format" );  
if( strcmp( Buf, "stack" )==0 ) format = STACK;
else format = CLUSTER;

DYtic = 1.0;

/* get bar shades */
gget( Buf, "Shade" ); 
sscanf( Buf, "%lf %lf %lf %lf %lf %lf %lf %lf", &s[0], &s[1], &s[2], &s[3], &s[4], &s[5], &s[6], &s[7] );

/* get zero line */
gget( Buf, "Zeroat" ); 
if( goodnum( Buf, &p )) zer = atof( Buf );
else zer = DXlo;

/* get label size */
if( label ) { gget( Buf, "Idfield.size" ); NTptsize( atof( Buf ) ) };

/* distance of label from bar top */
if( label ) { gget( Buf, "Idfield.position" ); lblpos = atof( Buf ); }

/* outline or not */
gget( Buf, "Outlinebars" ); if( Buf[0] == 'y' ) outline = YES; else outline = NO;

/* x distance between major bar spaces */
gget( Buf, "Separation" ); sep = atof( Buf );

gget( Buf, "Separation.sub" ); msep = atof( Buf );


yspace = ( (Yhi-Ylo)/((DYhi-DYlo)+1) ) - (sep*Scale_y);

if( format == CLUSTER ) nc = n; else nc = 1;

gget( Buf, "Killwild" ); /* option for aborting plot if any values out of range */
if( atof( Buf ) != 0 ) {
	for( i = 1; i <= N_d_rows; i++ ) {
		if( atof( D[i-1][ k[0] -1 ] ) > atof( Buf ) ) {
			fprintf( stderr, "Note: This Vbargraph terminated due to a value of %s.\n", D[i-1][k[0]-1] );
			return( 0 );
			}
		}
	}

gget( Buf, "Yfield" ); /* allow placement of bars by a data field */
if( strlen( Buf ) > 0 ) {
	yset = 1;
	yfld = atoi( Buf );
	if( yfld < 1 || yfld > N_d_fields ) { fprintf( stderr, "Yfield bad.\n" ); gdp_exit(); }
	}
else yset = 0;

gget( Buf, "Ystart.0or1" ); /* allow starting at 0 or 1 */
if( Buf[0] == '0' ) { start = 0; }
else { start = 1; }

for( i = 1; i <= N_d_rows; i++ ) {
	if( yset ) cury = da_y( atof( D[i-1][yfld-1] ) ) - (yspace/2);
	else cury = da_y((double)(DYlo+i+(start-1))) - (yspace/2);
	subspace = ( yspace / nc );
	for( j = 0; j < nc; j++ ) {
		if( !goodnum( D[i-1][ k[j]-1 ], &p )) {
			fprintf( stderr, "Warning: row %d, field %d, is bad (%s)\n", i, k[j], D[i-1][ k[j]-1] );
			cury += subspace;
			continue;
			}
		x = atof(D[i-1][ k[j]-1 ]);
		if( x != DXlo )
		    ab_rect( da_x(zer), cury, da_x(x), cury+(subspace-msep), s[j], (s[j]==1)?1:outline );
		if( label ) {
			if( x < zer || format == STACK ) f = (-lblpos)-Chh; else f = lblpos;
			strcpy( str, D[i-1][ idf[j]-1 ] );
			NTmov( da_x(x)+f, cury );
			NTtext( str ); /* will change */
			}
		if( format == STACK ) for( jj = 1; jj < n; jj++ ) {
			if( !goodnum( D[i-1][ k[jj] -1 ], &p ) ) {
				fprintf( stderr, "Warning: row %d, field %d, is bad (%s)\n", i, k[jj], D[i-1][k[jj]-1] );
				continue;
				}
			x2 = x + atof( D[i-1][ k[jj] -1 ] );
			if( x2 != DXlo )
			   ab_rect( da_x(x), cury,  da_x(x2), cury+(subspace-msep), s[jj], (s[jj]==1)?1:outline );
			if( label ) {
				if( x2 < zer || format == STACK ) f = (-lblpos)-Chh; else f = lblpos;
				NTmov( da_x(x2)+f, cury );
				strcpy( str, D[i-1][ idf[jj]-1 ] );
				NTtext( str );
				}
			x = x2;
			}

		cury += subspace;
		}
	}

gget( Buf, "Segment" );
if( Buf[0] == 'y' ) 
	for( f = DXlo+DXtic; f < DXhi; f += DXtic ) 
		rect(  f-(DXhi*0.003), DYlo + 0.2, f+(DXhi*0.004), DYhi - 0.2, (double)WHITE, 0 );
}
