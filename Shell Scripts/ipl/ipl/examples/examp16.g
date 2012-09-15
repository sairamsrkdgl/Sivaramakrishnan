Proc Page:
Title:	Time to Death for Study Patients
 	06-30-89

Proc Getdata:
Datafile: data16

Proc Areadef:	
Area:	lifetab
Xmax:	33
Xinc:	3
Xlabel:	Months of Follow-up
Ymax:	1.0
Yinc:	0.2
Yticfmt:	%3.1f
Ylabel:	Proportion of Patients Alive
Frame:	single
Xstub:	mps
Xstub.size:	8
Ystub.size:	8


Proc Lineplot:
Xfield:	1
Yfield:	2
Linetype:	0
Stairstep:	y

Proc Nicetab:
Area.rectangle:	2.0 4.7 6.0 9.4
Body.only:	y
Body.size:	8
File:	table16
