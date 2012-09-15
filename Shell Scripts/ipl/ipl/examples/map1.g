Proc Page:
Title:		Map example

Proc Areadef:
Area:	map
Xmin:	0
Xmax:	40
Xinc:	.5
# Xstub.grid:	line
Xstub.tics:	none
Xstub.stubs:	none
# Ystub.grid:	line
Ymax: 40
Ymin: 0
Yinc:		.5
Ystub.tics:	none
Ystub.stubs:	none
Frame:		double

Proc Getdata:
Datafile: data3

Proc Map:
Mode:	statedist
Statefield:	1
Threshold: 1 5 10 20 30 50 200
Shade: 1 .98 .85 .7 .55 .40 .25

Proc Legend:
Corner:	A
Shade: 1 .98 .85 .7 .55 .40 .25
Entry:	none
	1 - 4
	5 - 9
	10 - 19
	20 - 30
	30 - 49
	50 or more

Proc Getdata:
Datafile: data.cities

Proc Distribution:
Xfield: 1
Yfield: 2
Mark: sym6e
Mark.size: 10

Proc Text:
Embeddedcoords: y
File: data.cities
System: data
Size: 7
