# Uses rcrt file.  The format of this file is:
# dummy month smalls-enrolled meds-enrolled larges-enrolled med-goal large-goal yymm 
#   large-FEs med-FEs large-IPs med-IPs
# First 7 fields come from recruitment report plot.00 file, others gotten from 
# cross-tabling the inventory.

Proc Page:
Title:			Cumulative Patient Enrollment by Month
			Group A
			12-31-88

Proc Getdata:
Datafile:	data2

Proc Areadef:
Area:			whole
Ylabel:			Cumulative Number of Patients Enrolled
Xmax:			35
Xinc:			1
Ymax:			500
Yinc:			50
Xstub: 		month
Xstub.startmonth:		11
Xstub.startyear:		86
Xstart.0or1:		1
Shade: 0.95
Ystub.grid: wline

Proc Lineplot:
Yfield:		6 4 10 12
Accum:			y
Linetype:		1 0 4 7 
Linetype.magnify:		1 3 3 2
Label:			Goal
			Enrolled
			Failure to Enroll
			Ineligible
Xstart.0or1:		1
Linethick: 2.5 2.5 2.5 2.5 
