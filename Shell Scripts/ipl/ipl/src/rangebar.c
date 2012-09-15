#include "ipl.x"
Rangebar( )
{
int f[8], row, i, doends, nf, nv, ir, mlrightonly, label, xf;
double val[8];
double w, x, shade, lblpos, mlw;

gget( Buf, "Nval" );
nv = atoi( Buf );

gget( Buf, "Field" );
if( nv == 1 ) { nf = sscanf( Buf, "%d", &f[1] ); f[2] = f[3] = f[1]; f[4] = f[5] = f[1]; }
else if( nv == 2 ) { nf = sscanf( Buf, "%d %d", &f[1], &f[4] ); f[2] = f[3] = f[1]; f[5] = f[4]; }
else if( nv == 3 ) { nf = sscanf( Buf, "%d %d %d", &f[1], &f[3], &f[5] ); f[2] = f[1]; f[4] = f[5]; }
else if( nv == 4 ) { nf = sscanf( Buf, "%d %d %d %d", &f[1], &f[2], &f[4], &f[5] ); f[3] = f[4]; }
else if( nv == 5 ) { nf = sscanf( Buf, "%d %d %d %d %d", &f[1], &f[2], &f[3], &f[4], &f[5] ); }
if( nf != nv ) { fprintf( stderr, "Expecting %d Field values.\n", nv ); gdp_exit(); }


gget( Buf, "Width" ); w = atof( Buf );

gget( Buf, "Linethick" );
NTlinetype( "0", atof( Buf ), 1.0 );

label = 0;
gget( Buf, "Idfield" );
f[0] = atoi( Buf );
if( f[0] > 0 ) {
	label = 1;
	gget( Buf, "Label.size" );
	if( atoi( Buf ) > 0 ) NTptsize( atoi( Buf ) );

	gget( Buf, "Label.position" );
	lblpos = atof( Buf );

	}

doends = 0;
if( nf == 4 || nf == 5 ) {
	gget( Buf, "Ends" );
	if( Buf[0] == 'y' ) doends = 1;
	}

mlw = w;
mlrightonly = 0;
if( nf == 3 || nf == 5 ) {
	gget( Buf, "Midlinewidth" );
	if( atof( Buf ) > 0 )mlw = atof( Buf );
	gget( Buf, "Midlineright" );
	if( Buf[0] == 'y' ) mlrightonly = 1;
	}

gget( Buf, "Shade" );
shade = atof( Buf );

gget( Buf, "Xfield" );
xf = atoi( Buf );


x = DXlo;
for( ir = 0; ir < N_d_rows; ir++ ) {
	for( i = 1; i <= 5; i++ ) val[i] = atof( D[ ir ][ f[i]-1 ] );
	if( nv == 1 ) { val[1] = val[2] = 0; }
	if( xf > 0 ) x = atof( D[ir][ xf-1 ] );
	else x++;

	NTm( x-(w/2), val[2] ); /* lower edge of box */
	NTp( x+(w/2), val[2] );
	NTp( x+(w/2), val[4] );
	NTp( x-(w/2), val[4] );
	NTp( x-(w/2), val[2] ); /* upper edge */
	NTshade( shade );
	
	NTm( x, val[1] ); /* lower tail */
	NTl( x, val[2] );
	if( doends ) { NTm( x-(w/2.7), val[1] ); NTl( x+(w/2.7), val[1] ); }
	
	NTm( x-(w/2), val[2] ); /* lower edge of box */
	NTl( x+(w/2), val[2] );
	NTl( x+(w/2), val[4] );
	NTl( x-(w/2), val[4] );
	NTl( x-(w/2), val[2] ); /* upper edge */
	
	NTm( x, val[4] );
	NTl( x, val[5] ); /* upper tail */
	if( doends ) { NTm( x-(w/2.7), val[5] ); NTl( x+(w/2.7), val[5] ); }
	
	if( mlrightonly )NTm( x-(w/2), val[3] ); /* median line */
	else NTm( x-(mlw/2), val[3] ); 
	NTl( x+(mlw/2), val[3] );
	
	if( label ) {
		NTmov( da_x(x) -1, da_y(lblpos) );  /* print label */
		sprintf( Buf, "%s", D[ir][f[0]-1 ] );
		NTcentext( Buf, 2 );
		}
	
	}
NTnormline();
}
