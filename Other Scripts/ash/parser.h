/*
 * Copyright (C) 1989 by Kenneth Almquist.  All rights reserved.
 * This file is part of ash, which is distributed under the terms specified
 * by the Ash General Public License.  See the file named LICENSE.
 */

/* control characters in argument strings */
#define CTLESC '\201'
#define CTLVAR '\202'
#define CTLENDVAR '\203'
#define CTLBACKQ '\204'
#define CTLQUOTE 01		/* ored with CTLBACKQ code if in quotes */

/* variable substitution byte (follows CTLVAR) */
#define VSTYPE 07		/* type of variable substitution */
#define VSNUL 040		/* colon--treat the empty string as unset */
#define VSQUOTE 0100		/* inside double quotes--suppress splitting */

/* values of VSTYPE field */
#define VSNORMAL 1		/* normal variable:  $var or ${var} */
#define VSMINUS 2		/* ${var-text} */
#define VSPLUS 3		/* ${var+text} */
#define VSQUESTION 4		/* ${var?message} */
#define VSASSIGN 5		/* ${var=text} */


/*
 * NEOF is returned by parsecmd when it encounters an end of file.  It
 * must be distinct from NULL, so we use the address of a variable that
 * happens to be handy.
 */
extern int tokpushback;
#define NEOF ((union node *)&tokpushback)


#ifdef __STDC__
union node *parsecmd(int);
int goodname(char *);
#else
union node *parsecmd();
int goodname();
#endif
