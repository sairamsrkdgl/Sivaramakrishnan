# Uses rcrt file.  The format of this file is:
# dummy month smalls-enrolled meds-enrolled larges-enrolled med-goal large-goal yymm 
#   large-FEs med-FEs large-IPs med-IPs
# First 7 fields come from recruitment report plot.00 file, others gotten from 
# cross-tabling the inventory.
Proc Page:
Title:			Monthly Patient Recruitment by Group
			12-31-88

Proc Getdata:
Datafile:		data2

Proc Areadef:
Area:			2hi
Subtitle:		Group A
Xstub:	 		month
Xstub.startmonth:	11
Xstub.startyear:	86
Subtitle.justify:	left
Subtitle.above:		0.02
Ymax:			25
Yinc:			5
Xmax:			35
Xstart.0or1:		1

Proc Bargraph:
Field:			5
Shade:			0.8
Idfield:		5

Proc Lineplot:
Yfield:			7
Linetype:		1
Linetype.magnify:	1
Stairstep.bars:		y
Xstart.0or1:		1

Proc Legend:
Entry:			Monthly Goal
Linetype:		1
Linetype.magnify:	1
Corner:			B

Proc Areadef:
Area:			2lo
Subtitle:		Group B
Xstub:	 		month
Xstub.startmonth:	11
Xstub.startyear:	86
Subtitle.justify:	left
Subtitle.above:		0.02
Ymax:			25
Yinc:			5
Xmax:			35
Xstart.0or1:		1

Proc Bargraph:
Field:			4
Idfield:		4
Shade:			0.8

Proc Lineplot:
Yfield:		6
Linetype:		1
Linetype.magnify:	1
Stairstep.bars:		y
Xstart.0or1:		1
