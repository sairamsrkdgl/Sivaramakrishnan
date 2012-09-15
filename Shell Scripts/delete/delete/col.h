/*
 * $Source: /afs/athena.mit.edu/astaff/project/delete/src/RCS/col.h,v $
 * $Author: jik $
 *
 * This header file is part of a package including delete, undelete,
 * lsdel, expunge and purge.  The software suite is meant as a
 * replacement for rm which allows for file recovery.
 * 
 * Copyright (c) 1989 by the Massachusetts Institute of Technology.
 * For copying and distribution information, see the file "mit-copying.h."
 */
#include "mit-copying.h"

/*
 * DEF_COL_WIDTH: the column with to try to use if none is specified.
 * DEF_WAIT: 1 if the program is supposed to wait for stdin to get to
 *           end-of-file and then print out everything in order in
 *           columns by default.  If this is 0, then the default is
 *           for the program to print across instead of down and to
 *           print as it receives input from stdin.
 * DEF_VAR_COLS: if 1, use variable-width columns based on text width.
 *               if 1, DEF_WAIT must be true.
 * DEF_SCR_WIDTH: default screen width
 * DEF_NUM_ITEMS: if 1, number each item
 * DEF_MARGIN: the default margin in between columns of text
 */ 
#define DEF_COL_WIDTH 20
#define DEF_WAIT 1
#define DEF_VAR_COLS 1
#define DEF_SCR_WIDTH 80
#define DEF_NUM_ITEMS 1
#define DEF_MARGIN 2
 /* This is used for when we need a guess as to how long a number will */
 /* be when printed.  Also, if we are supposed to work in wait mode    */
 /* and are not given a maxitems value, this is what is used.          */
#define DEF_MAX_ITEMS 10000
