#include "ipl.x"

Pie( )
{
int i, f, first, ns, explode;
double theta, val, stop;
double hx, hy, x, y, r;
double sh[20], lt;
double u, ux, uy, adj;

gget( Buf, "Field" );
f = atoi( Buf );

gget( Buf, "Center" );
sscanf( Buf, "%lf %lf", &hx, &hy );

gget( Buf, "Radius" );
r = atof( Buf );

gget( Buf, "Linethick" );
lt = atof( Buf );
NTlinetype( "0", lt, 1.0 );

gget( Buf, "Explode" );
u = atof( Buf );

gget( Buf, "Rotate" );
adj = atof( Buf );

gget( Buf, "Shade" );
ns = sscanf( Buf, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
  &sh[0],&sh[1],&sh[2],&sh[3],&sh[4],&sh[5],&sh[6],&sh[7],&sh[8],&sh[9],
  &sh[10],&sh[11],&sh[12],&sh[13],&sh[14],&sh[15],&sh[16],&sh[17],&sh[18],&sh[19] );

/* do shades */
theta = adj * (3.1415927/180.0) ;
for( i = 0; i < N_d_rows; i++ ) {

	val = atof( D[i][f-1] );
	
	/* convert val (percentage) to radians.. */
	val = ( val/100.0 ) * 6.28319;
	stop = theta + val;
	ux = u * cos( theta+(val/2.0) );
	uy = u * sin( theta+(val/2.0) );

	first = 1;
	for( ; theta < (stop+0.001); theta += 0.02 ) {
		x = hx + (r * cos( theta ));
		y = hy + (r * sin( theta ));
		if( first ) { NTmov( hx+ux, hy+uy ); NTpath( x+ux, y+uy ); first = 0; }
		NTpath( x+ux, y+uy );
		}
	if( i < ns )NTshade( sh[i] );
	else NTshade( 1.0 );
	theta = stop;
	}

/* do lines */
theta = adj * (3.1415927/180.0);
for( i = 0; i < N_d_rows; i++ ) {

	val = atof( D[i][f-1] );
	
	/* convert val (percentage) to radians.. */
	val = ( val/100.0 ) * 6.28319;
	stop = theta + val;
	ux = u * cos( theta+(val/2.0) );
	uy = u * sin( theta+(val/2.0) );

	first = 1;
	for( ; theta < (stop+0.001); theta += 0.02 ) {
		x = hx + (r * cos( theta ));
		y = hy + (r * sin( theta ));
		if( first ) { NTmov( hx+ux, hy+uy ); NTlin( x+ux, y+uy ); first = 0; }
		NTlin( x+ux, y+uy );
		}
	NTlin( hx+ux, hy+uy );
	theta = stop;
	}

NTnormline();
}
