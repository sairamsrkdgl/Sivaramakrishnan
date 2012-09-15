#include "ipl.x"
Vrangebar( )
{
int f[8], row, i, doends, nf, nv, ir, mlrightonly, label, yf;
double val[8];
double w, y, shade, lblpos, mlw;

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

gget( Buf, "Yfield" );
yf = atoi( Buf );


y = DYlo;
for( ir = 0; ir < N_d_rows; ir++ ) {
	for( i = 1; i <= 5; i++ ) val[i] = atof( D[ ir ][ f[i]-1 ] );
	if( nv == 1 ) { val[1] = val[2] = 0; }
	if( yf > 0 ) y = atof( D[ir][ yf-1 ] );
	else y++;

	NTm( val[2], y-(w/2) ); /* lower edge of box */
	NTp( val[2], y+(w/2) );
	NTp( val[4], y+(w/2) );
	NTp( val[4], y-(w/2) );
	NTp( val[2], y-(w/2) ); /* upper edge */
	NTshade( shade );
	
	NTm( val[1], y ); /* lower tail */
	NTl( val[2], y );
	if( doends ) { NTm( val[1], y-(w/2.7) ); NTl( val[1], y+(w/2.7) ); }
	
	NTm( val[2], y-(w/2) ); /* lower edge of box */
	NTl( val[2], y+(w/2) );
	NTl( val[4], y+(w/2) );
	NTl( val[4], y-(w/2) );
	NTl( val[2], y-(w/2) ); /* upper edge */
	
	NTm( val[4], y );
	NTl( val[5], y ); /* upper tail */
	if( doends ) { NTm( val[5], y-(w/2.7) ); NTl( val[5], y+(w/2.7) ); }
	
	if( mlrightonly )NTm( val[3], y-(w/2) ); /* median line */
	else NTm( val[3], y-(mlw/2) ); 
	NTl( val[3], y+(mlw/2) );
	
	if( label ) {
		NTmov( da_x(lblpos)-1, da_y(y) );  /* print label */
		sprintf( Buf, "%s", D[ir][f[0]-1 ] );
		NTcentext( Buf, 2 );
		}
	
	}
NTnormline();
}
