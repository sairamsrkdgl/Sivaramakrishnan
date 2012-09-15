#include "ipl.x"

Arrow( )
{
double x1, y1, x2, y2;
gget( Buf, "Points" );
sscanf( Buf, "%lf %lf %lf %lf", &x1, &y1, &x2, &y2 );
arr( x1, y1, x2, y2, 0.3, 0.2, 0.0 );
}


arr( x1, y1, x2, y2, delt_th, r, sh )
double x1, y1, x2, y2, delt_th, r, sh;
{
double vx, vy, ax1, ay1, ax2, ay2, th0, th1, th2, atan();
vx = x2 - x1;
vy = y2 - y1;

th0 = atan( vy / vx );
th1 = th0 + delt_th;
th2 = th0 - delt_th;

if( x2 < x1 ) {
	ax1 = x2 + (r * cos( th1 ));
	ay1 = y2 + (r * sin( th1 ));
	ax2 = x2 + (r * cos( th2 ));
	ay2 = y2 + (r * sin( th2 ));
	}
else 	{
	ax1 = x2 - (r * cos( th1 ));
	ay1 = y2 - (r * sin( th1 ));
	ax2 = x2 - (r * cos( th2 ));
	ay2 = y2 - (r * sin( th2 ));
	}

NTmov( x2, y2 );
NTpath( ax1, ay1 );
NTpath( ax2, ay2 );
NTshade( sh );
NTmov( x1, y1 );
NTlin( x2, y2 );
}
