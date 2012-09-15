Proc Page:
Title:		Number of Records Logged/Entered at Center per Month
		February 28, 1989

Proc Areadef:
Xmin:	0
Xmax:	30
Xinc:	1
Xstub:	month
Xstart.0or1:		1
Xstub.startmonth:	1
Xstub.startyear:	87	
Xstub.grid:		line	
Ystub.grid:	line	
Ymin:	0
Ymax:	2000
Yinc:		200
Ystub: num
Xlabel:		Month and year
Ylabel:		Number of records, Accum. =  27,305

Proc Getdata:
Datafile:	data0

Proc Lineplot:
Yfield:		2
Linethick:	2
Stairstep:	y
Numbers:	y
Label.size:	6
Xstart.0or1:	1
