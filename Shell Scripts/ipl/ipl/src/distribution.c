/* distribution() - produces scatter plots.  If cluster is passed
	as YES, duplicate values will cluster around the point.
	Data must be sorted numerically for this to work.  If
	cluster == NO, duplicate points will overstrike.
*/
#include "ipl.x"

Distribution( )
{
int 	i, j, 
	dups, 
	cluster, 
	p, 
	dist, 
	justdist,
	xfield, 
	yfield, 
	idfield,
	sizefield,
	shadefield,
	markfield;
double 	adjx, adjy, 
	xdat, ydat,
	x, y, 
	prevx, prevy, 
	charh, charv, 
	size,
	sizescale,
	shade,
	shadescale,
	distlen;
char 	c[20];
FILE 	*fp, 
	*fp2;
static double xofst[38] = { 0, 0, 4, 0, -4, 4, -4, -4, 4,
	0, -8, 0, 8, 4, -8, 4, 8, -4, -8, -4, 8,
	0, 0, 12, -12, 4, 4, 12, -12, -4, -4, 12, -12,
	8, -8, -8, 8 };
static double yofst[38] = { 0, 4, 0, -4, 0, 4, -4, 4, -4,
	-8, 0, 8, 0, -8, 4, 8, 4, -8, -4, 8, -4,
	12, -12, 0, 0, 12, -12, 4, 4, 12, -12, -4, -4,
	8, -8, 8, -8 };
static double distofst[38] = { 0, 1, -1, 2, -2, 3, -3, 4, -4, 5, -5, 6, -6, 7, -7, 8, -8, 
	9, -9, 10, -10, 11, -11, 12, -12, 13, -13, 14, -14, 15, -15, 16, -16, 17, -17, 18, -18 };

gget( Buf, "Xfield" ); xfield = atoi( Buf );
gget( Buf, "Yfield" ); yfield = atoi( Buf );
if( xfield < 1 ) { fprintf( stderr, "1 or 2 fdata fields need to be given for distributions.\n" ); gdp_exit(); }
if( yfield < 1 ) {
	justdist = YES; 
	yfield = xfield;
	}
else justdist = NO;
gget( Buf, "Idfield" ); idfield = atoi( Buf );
gget( Buf, "Mark.field" ); markfield = atoi( Buf );

gget( c, "Mark" );
gget( Buf, "Mark.font" ); NTfont( Buf ); 
gget( Buf, "Mark.size" ); NTptsize( atof( Buf ) );
charv = Chh / 2.0;
charh = Chh / 4.0;
size = Chh * 0.4;
gget( Buf, "Cluster" ); if( Buf[0] == 'y' ) cluster = YES; else cluster = NO;
dist = NO;
gget( Buf, "Axdist" ); 
if( Buf[0] == 'y' ) { 
	dist = YES; 
	gget( Buf, "Axdist.length" ); distlen = atof( Buf );
	}
gget( Buf, "Sizefield" ); sizefield = atoi( Buf );
gget( Buf, "Sizescale" ); sizescale = atof( Buf );
gget( Buf, "Shadefield" ); shadefield = atoi( Buf );
gget( Buf, "Shadescale" ); shadescale = atof( Buf );

if( cluster ) {		/* sort the data numerically */
	/* write out data */
	fp = fopen( Tempfile, "w" );
	if( fp == NULL ) fprintf( stderr, "Distribution can't open: %s", Tempfile );	
	for( i = 0; i < N_d_rows; i++ ) {
		for( j = 0; j < N_d_fields; j++ ) fprintf( fp, "%s ", D[i][j] );
		fprintf( fp, "\n" );
		}
	fclose( fp );
	/* build Unix sort command */
	sprintf( Buf, "sort -n %s +%d -%d +%d -%d -o %s", Tempfile, xfield-1, xfield, yfield-1, yfield, Tempfile );
	system( Buf );
	fp2 = fopen( Tempfile, "r" );
	for( i = 0; i < N_d_rows; i++ ) {
		for( j = 0; j < N_d_fields; j++ ) { fscanf( fp2, "%s", D[i][j] ); }
		}
	fclose( fp2 );
	}

x = -99999.0;
y = -99999.0;
dups = 0;
for( i = 0; i < N_d_rows; i++ ) {
	if( (!goodnum( D[i][yfield-1], &p ) && !justdist) || !goodnum( D[i][xfield-1], &p )) {
		if( idfield ) fprintf( stderr, "%s ", D[i][idfield-1] );
		else fprintf( stderr, "row %d ", i ); 
 		fprintf( stderr, "is bad: (%s,%s) (Warning)\n", D[i][xfield-1], D[i][yfield-1] );
		continue;
		}
	x = da_x( atof( D[i][xfield-1] ) );
	y = da_y( atof( D[i][yfield-1] ) );
	if( (!justdist && (y < Ylo || y > Yhi )) || x < Xlo || x > Xhi ) { 
		if( idfield )fprintf( stderr, "%s ", D[i][idfield-1] );
		else fprintf( stderr, "Row %d ", i );
		fprintf( stderr, "is out of bounds: (%s,%s) (Warning)\n", D[i][xfield-1], D[i][yfield-1] );
		continue; 
		}  
	if( cluster && (x == prevx && y == prevy ) ) {
		dups++;
		if( dups > 36 ) dups = 1;
		if( justdist ) adjx = x + (.01*distofst[dups]);
		else adjx = x + (.01*xofst[dups]);
		adjy = y + (.01*yofst[dups]);
		}
	else	{
		dups = 0;
		prevx = x;
		prevy = y;
		adjx = x;
		adjy = y;
		}
	if( !justdist ) {
		if( sizefield > 0 && sizefield <= 24 ) {
			size = ( atof( D[i][sizefield-1] ) * sizescale) / 144.0; 
			if( size < 0.001 || size > 3 ) {
				fprintf( stderr, "warning, rec. %d, abnormal size data value\n", i  ); 
				}
			}
		if( shadefield > 0 && shadefield <= 24 ) {
			shade = atof( D[i][shadefield-1] ) * shadescale;
			if( shade < 0 || shade > 1 ) {
				fprintf( stderr, "warning, rec %d, abnormal shade data value, truncated.\n", i );
				if( shade < 0 ) shade = 0;
				if( shade > 1 ) shade = 1;
				}
			sprintf( c, "%s%4.2f", c, shade );
			}
			
			
		if( strncmp( c, "sym", 3 )==0 ) {
			point( adjx, adjy, c, size );
			c[5] = '\0'; /* get rid of shade if any */
			}
		if( markfield || strncmp( c, "sym", 3 )!=0 ) {
			NTmov( (adjx-(charh*0.20))-1, adjy-(charv*0.48) );
			if( markfield ) NTcentext( D[i][markfield-1], 2 );
			else NTcentext( c, 2 );
			}
		}
	else if( justdist ) {
		NTmov( adjx, Ylo ); NTlin( adjx, Yhi );
		}
	if( dist && !justdist ) {
		NTmov( Xhi, adjy ); NTlin( Xhi+distlen, adjy );
		NTmov( adjx, Yhi ); NTlin( adjx, Yhi+distlen );
		}
	}
}
