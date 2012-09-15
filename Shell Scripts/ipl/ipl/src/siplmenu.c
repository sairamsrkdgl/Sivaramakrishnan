#include "ipl.x"
siplmenu( proc )
char proc[];
{
char p1[80], p2[80];

if( strcmp( proc, "Endoffile" )==0 ) while( 1 ) {
	message( Controlfile,
		"Press RETURN for next file,",
		"enter another file to view,",
		"or enter 'q' to quit." );
	get_string_box( p2, "> ", 8.0, 9.5, 11.0, 9.8 );
	if( strcmp( p2, "q" )==0 ) { gdp_exit(); }
	else	{
		message( "Next control file:", p2, "", "" );
		if( ! restart( p2 ) ) { sleep( 3 ); continue; }
		NTshow();
		break;
		}
	}

if( strcmp( proc, "Pagebreak" )==0 ) {
	message( "SIPL graphics previewer", "", "More..", "Press RETURN.." );
	get_string_box( p2, "> ", 8.0, 9.5, 11.0, 9.8 );
	}
}
