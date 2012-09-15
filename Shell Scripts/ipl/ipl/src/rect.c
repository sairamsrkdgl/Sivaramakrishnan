/* do a rectangle, with shading and/or outline */
/* only used internally, user proc is Polygon */
#include "ipl.x"

/* rect() - all coords in data space */
rect( xlo, ylo, xhi, yhi, shade, outline )
double xlo, ylo, xhi, yhi, shade;
int outline;
{
if( shade >= 0 ) {
	NTm( xlo, ylo );
	NTp( xlo, yhi );
	NTp( xhi, yhi );
	NTp( xhi, ylo );
	NTshade( shade );
	}
if( outline ) {
	NTm( xlo, ylo );
	NTl( xlo, yhi );
	NTl( xhi, yhi );
	NTl( xhi, ylo );
	NTl( xlo, ylo );
	}
}

/* ab_rect() - all coords in absolute space */
ab_rect( xlo, ylo, xhi, yhi, shade, outline )
double xlo, ylo, xhi, yhi, shade;
int outline;
{
if( shade >= 0 ) {
	NTmov( xlo, ylo );
	NTpath( xlo, yhi );
	NTpath( xhi, yhi );
	NTpath( xhi, ylo );
	NTshade( shade );
	}
if( outline ) {
	NTmov( xlo, ylo );
	NTlin( xlo, yhi );
	NTlin( xhi, yhi );
	NTlin( xhi, ylo );
	NTlin( xlo, ylo );
	}
}
