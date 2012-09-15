#!/bin/sh
#  cgrep - context grep via sed
#  Written by Greg Ubben, DoD, 10 Dec 1993
#
#  Cgrep finds all occurrences of <pattern> in <file...>, showing <context>
#  additional lines of context above and below each occurrence.  <context>
#  must be at least 1, and defaults to 2.  Each new block of context is
#  preceded by the line number of the first occurrence within that block.
#  <pattern> must be a grep-style regular expression, except that you can
#  match across lines, so use "\nWORD" vs "^WORD" and "WORD\n" vs "WORD$".
#
#  The following example will find all occurrences of the word "five" where
#  it is followed by the word "seven" somewhere within the next 3 lines:
#	cgrep -3 "five.*seven"

USAGE="$0 [-context] pattern [file...]"

n=3
case $1 in -[1-9]*)
	n=`expr 1 - "$1"`
	shift
esac
re=${1?}; shift

sed -n "
	1b start
	: top
	\~$re~{
		h; n; p; H; g
		b endif
	}
		N
		: start
		//{ =; p; }
	: endif
	$n,\$D
	b top
" "$@"
