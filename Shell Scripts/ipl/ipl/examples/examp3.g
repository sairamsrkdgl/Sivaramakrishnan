# line plot
Proc Page:
Title:	Major Contracts Won, by Division
	For 1988 and 1st Quarter 1989

Proc Getdata:
Data:	1st~Quarter~1988	27 5 32
	2nd~Quarter~1988	15 14 82
	3rd~Quarter~1988	17 15 43
	4th~Quarter~1988	37 9 31
	1st~Quarter~1989	45 11 32

Proc Areadef:
Xmax:	7
Xinc:	1
Xstub:	@1
Xstub.stubs:	both
Xstub.grid:	line
Xstart.0or1: 1
Ymax:	100
Yinc:	10
Ylabel:	Number of Major Contracts
Xlabel: Time Period

Proc Lineplot:
Yfield:		2 3 4
Xstart.0or1:	1
Linetype:	0 1 2
Label:		South Bend
		Grand Rapids
		Lockport
Mark:		sym1e sym4e sym6e



Proc Legend:
Location:	5 2
Entry:	South Bend, John Preston
	Grand Rapids, Felix Catt
	Lockport, Ed Parker
Linetype:	0 1 2	
Mark:		sym1e sym4e sym6e
