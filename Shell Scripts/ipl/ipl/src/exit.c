#include "ipl.x"
Exit( )
{
if( !Hold )NTshow();
unlink( Tempfile );
}
