/* proc_call() - calls the appropriate funtion, given the proc name. */

#include "ipl.h"
proc_call( proc )
char proc[];
{
char p1[30], p2[30];

/* make sure there's data if we're plotting */
if( smember( proc, "Distribution Bargraph Vbargraph Lineplot Rangebar Vrangebar Vector Boxplot Errorbars Map" ) ) {
	if( strcmp( proc, "Map" )!= 0 && N_d_rows <= 0 ) 
		{ fprintf( stderr, "No data yet, use Proc Getdata to read some.\n" ); gdp_exit(); }
	if( Xlo == 0 && Xhi == 0 ) 
		{ fprintf( stderr, "No graphics area, use Proc Areadef to define one.\n" ); gdp_exit(); }
	}

if( Dev == 's' ) siplmenu( proc ); /* sipl menu at end of page */

if( strcmp( proc, "Initialize" )!= 0 )NTbatch_on();

if( strcmp( proc, "Initialize" )==0 ) Initialize( );
else if( strcmp( proc, "Exit" )==0 ); /* see below */
else if( strcmp( proc, "Page" )==0 ) Page( );
else if( strcmp( proc, "Areadef" )==0 ) Areadef( );
else if( strcmp( proc, "Getdata" )==0 ) Getdata( );
else if( strcmp( proc, "Distribution" )==0 ) Distribution( );
else if( strcmp( proc, "Bargraph" )==0 ) Bargraph( );
else if( strcmp( proc, "Vbargraph" )==0 ) Vbargraph( );
else if( strcmp( proc, "Lineplot" )==0 ) Lineplot( );
else if( strcmp( proc, "Legend" )==0 ) Legend( );
else if( strcmp( proc, "Map" )==0 ) Map( );
else if( strcmp( proc, "Rangebar" )==0 ) Rangebar();
else if( strcmp( proc, "Vrangebar" )==0 ) Vrangebar();
else if( strcmp( proc, "Vector" )==0 ) Vector();
else if( strcmp( proc, "Pie" )==0 ) Pie();
else if( strcmp( proc, "Errorbars" )==0 ) Errorbars();
else if( strcmp( proc, "Nicetab" )==0 ) Nicetab( );
else if( strcmp( proc, "Boxplot" )==0 ) Boxplot( );

else if( strcmp( proc, "Text" )==0 ) Text( );
else if( strcmp( proc, "Draw" )==0 ) Draw( );
else if( strcmp( proc, "Arrow" )==0 ) Arrow( );
else if( strcmp( proc, "Polygon" )==0 ) Polygon( );
else if( strcmp( proc, "Point" )==0 ) Point();

else if( strcmp( proc, "Pagebreak" )== 0 ) { NTshow(); }
else if( strcmp( proc, "QuitAppend" )==0 ) { Hold = 1; }
else if( strcmp( proc, "Endoffile" )==0 ) ; /* no op */
else fprintf( stderr, "proc_call: %s is unrecognized.. continuing\n", proc );

NTbatch_off();

if( strcmp( proc, "Exit" )==0 ) Exit();
}
