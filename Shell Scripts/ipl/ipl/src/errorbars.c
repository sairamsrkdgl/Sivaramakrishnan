#include "ipl.x"

Errorbars( )
{
int i, xfld, yfld, efld, dubl;
double x, y, err, tlen, ofs;

gget( Buf, "Xfield" );
xfld = atoi( Buf );
if( xfld < 1 || xfld > 24 ) { fprintf( stderr, "Xfield bad.\n" ); gdp_exit(); }

gget( Buf, "Yfield" );
yfld = atoi( Buf );
if( yfld < 1 || yfld > 24 ) { fprintf( stderr, "Yfield bad.\n" ); gdp_exit(); }

gget( Buf, "Errfield" );
efld = atoi( Buf );
if( efld < 1 || efld > 24 ) { fprintf( stderr, "Errfield bad.\n" ); gdp_exit(); }

gget( Buf, "Offset" );
ofs = atof( Buf );

gget( Buf, "Double" );
if( Buf[0] == 'y' ) dubl = 1;
else dubl = 0;

gget( Buf, "Linethick" );
if( strlen( Buf ) > 0 ) NTlinetype( "0", atof( Buf ), 1.0 );

gget( Buf, "Taillen" );
if( strlen( Buf ) > 0 ) tlen = atof( Buf );

for( i = 1; i <= N_d_rows; i++ ) {
	x = atof( D[i-1][xfld-1] ) ;
	y = atof( D[i-1][yfld-1] );
	err = atof( D[i-1][efld-1] );
	if( dubl ) err *= 2.0;

	NTmov( da_x(x)+ofs, da_y(y) ); /* top bar */
	NTlin( da_x(x)+ofs, da_y(y+err) );
	NTmov( (da_x(x)-(tlen/2))+ofs, da_y(y+err) ); /* tail */
	NTlin( da_x(x)+(tlen/2)+ofs, da_y(y+err) );
	
	NTmov( da_x(x)+ofs, da_y(y) ); /* bottom bar */
	NTlin( da_x(x)+ofs, da_y(y-err) );
	NTmov( (da_x(x)-(tlen/2))+ofs, da_y(y-err) ); /* tail */
	NTlin( da_x(x)+(tlen/2)+ofs, da_y(y-err) );
	
	}

NTnormline();
}
