/*
 * $Source: /afs/athena.mit.edu/astaff/project/delete/src/RCS/pattern.h,v $
 * $Author: jik $
 * $Header: /afs/athena.mit.edu/astaff/project/delete/src/RCS/pattern.h,v 1.6 91/02/28 18:43:19 jik Exp $
 * 
 * This program is part of a package including delete, undelete,
 * lsdel, expunge and purge.  The software suite is meant as a
 * replacement for rm which allows for file recovery.
 * 
 * Copyright (c) 1989 by the Massachusetts Institute of Technology.
 * For copying and distribution information, see the file "mit-copying.h."
 */
#include "mit-copying.h"

int add_str();
int add_arrays();
int find_contents();
int find_deleted_contents();
int find_deleted_contents_recurs();
int find_matches();
int find_deleted_matches();
int find_recurses();
int find_deleted_recurses();

#define FIND_DELETED		(1<<0)
#define FIND_UNDELETED		(1<<1)
#define RECURS_FIND_DELETED	(1<<2)
#define RECURS_FIND_UNDELETED	(1<<3)
#define RECURS_DELETED		(1<<4)
#define FOLLW_LINKS		(1<<5)
#define FOLLW_MOUNTPOINTS	(1<<6)
#define FIND_DOTFILES		(1<<7)
#define FIND_CONTENTS		(1<<8)
#define SUPPRESS_WARNINGS	(1<<9)
#define RECURS			(RECURS_FIND_DELETED | RECURS_FIND_UNDELETED |\
				 RECURS_DELETED | FIND_CONTENTS)
