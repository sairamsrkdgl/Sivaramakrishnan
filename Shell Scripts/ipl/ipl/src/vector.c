#include "ipl.x"

Vector( )
{
int i, fx1, fy1, fx2, fy2;
double hl, hw, hs;

gget( Buf, "Point1fields" ); sscanf( Buf, "%d %d", &fx1, &fy1 );
gget( Buf, "Point2fields" ); sscanf( Buf, "%d %d", &fx2, &fy2 );
gget( Buf, "Headlength" ); hl = atof( Buf );
gget( Buf, "Headwidth" ); hw = atof( Buf );
gget( Buf, "Headshade" ); hs = atof( Buf );

for( i = 0; i < N_d_rows; i++ ) {
	arr( da_x( atof( D[i][fx1-1] ) ), da_y( atof( D[i][fy1-1] ) ),
	     da_x( atof( D[i][fx2-1] ) ), da_y( atof( D[i][fy2-1] ) ),
		hw, hl, hs );
	}
}
