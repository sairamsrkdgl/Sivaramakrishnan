#include "ipl.x"
Boxplot( )
{
int f[8], row, i, doends;
double val[8];
double w, x;

gget( Buf, "Linethick" );
NTlinetype( "0", atof( Buf ), 1.0 );

gget( Buf, "Label.size" );
if( atoi( Buf ) > 0 ) NTptsize( atoi( Buf ) );

gget( Buf, "Ends" );
if( Buf[0] == 'y' ) doends = 1;
else doends = 0;

gget( Buf, "Fields" );
if( strlen( Buf ) > 0 ) {
	if( sscanf( Buf, "%d %d %d %d %d %d", &f[0], &f[1], &f[2], &f[3], &f[4], &f[5] ) != 6 ) {
		fprintf( stderr, "Fields should specify six data fields (n, and the 5,25,50,75,95 percentiles)\n" );
		gdp_exit();
		}
	}

gget( Buf, "Datarow" );

row = atoi( Buf );
for( i = 0; i < 6; i++ ) {
	if( f[i] < 1 || f[i] > N_d_fields ) { fprintf( stderr, "Pfields bad.\n" ); gdp_exit(); }
	else if( row > 0 && row <= N_d_rows ) val[i] = atof( D[ row - 1 ][ f[i]-1 ] );
	else { fprintf( stderr, "Datarow bad.\n" ); gdp_exit(); }
	}

gget( Buf, "Xloc" ); x = atof( Buf );
gget( Buf, "Width" ); w = atof( Buf );

NTm( x-(w/2), val[2] ); /* lower edge of box */
NTp( x+(w/2), val[2] );
NTp( x+(w/2), val[4] );
NTp( x-(w/2), val[4] );
NTp( x-(w/2), val[2] ); /* upper edge */
NTshade( 1.0 );

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

NTm( x-(w/2), val[3] ); /* median line */
NTl( x+(w/2), val[3] );

gget( Buf, "Printn" );
if( Buf[0] == 'y' ) {
	NTmov( da_x(x) -1, Ylo-0.15 );  /* print N */
	sprintf( Buf, "N = %d", (int) (val[0]) );
	NTcentext( Buf, 2 );
	}

NTnormline();
}
