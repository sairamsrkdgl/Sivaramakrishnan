Proc Page:
Title:		Disposition of Patients Evaluated with Asbestosis
		by Region and Trial Group
		06-30-89

Proc Getdata:
Datafile:	data4

Proc Areadef:
Subtitle:	
Area:		2hi
Xmax: 		37
Ymax:		50
Yinc: 5
Xstart.0or1:	1
Subtitle.above:	0.02
Subtitle.justify:	left
Xlabel:		Region (A = affiliate)
Ylabel:		Number of Patients
Xstub:		data4A

Proc Bargraph:
Field:		8 2 5
Shade:		0.4 0.8 1
Format:		stack

Proc Legend:
Entry:		Enrolled
		Eligible but Failed to Enroll
		Ineligible
Shade:		0.4 0.8 1

Proc Areadef:
Subtitle:	Medium Tumors
Area:		2lo
Xmax: 		37
Ymax:		50
Yinc: 	5
Xstart.0or1:	1
Subtitle.above:	0.02
Subtitle.justify:	left
Xlabel:		Region (A = affiliate)
Ylabel:		Number of Patients
Xstub:		data4A

Proc Bargraph:
Field:		9 3 6
Shade:		0.4 0.8 1
Format:		stack
