/*
 * value of $KSH_VERSION
 */

static char *RCSid = "$Header: version.c,v 3.1 88/11/03 09:18:36 egisin Exp $";

#include <stddef.h>
#include <setjmp.h>
#include "sh.h"

char ksh_version [] =
	"KSH_VERSION=@(#) PD alpha $Revision: 3.1 $ $Date: 88/11/03 09:18:36 $";

/***
$Log:	version.c,v $
Revision 3.1  88/11/03  09:18:36  egisin
alpha distribution

Revision 1.3  88/10/20  17:34:03  egisin
added @(#) to ksh_version

Revision 1.2  88/09/27  19:01:58  egisin
fix version.c

Revision 1.1  88/09/27  18:59:06  egisin
Initial revision
***/

