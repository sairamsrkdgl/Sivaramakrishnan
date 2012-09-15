Proc Page:
Paperway:	landscape
Title:	Clinical Trial Enrollment
	== Goals and Progress ==
Standard.linethick:	0.3

Proc Getdata:
# data entered by hand from checking each monthly recruitment report
# since the beginning of the study.  Subsequent runs can just append recent
# data.
Data:	0
5
2
4
11
6
5
8
9
10
9
10
11
5
8
13
11
12
6
12
14
5
7
5
12
7
11
9
3
14
7
7

Proc Areadef:
Xmax:	96
Ymax:	2500
Yinc:	500
Ylabel: Cumulative Enrollment
Xlabel:	Calendar Year, By Quarter
Xstub.tics: none
Xstub.stubs: none

Proc Lineplot:
Yfield:  1
Accum:	y
Label: Actual Enrollment

Proc Draw:
Linethick:	0.4
Linetype.magnify:  6
System: data
Saveas: A
Points:	0 0 96 2208

Proc Draw:
Clone:	A
Linetype:	3
Points: 0 0 96 1536

Proc Draw:
Clone:	A
Linetype:	8
Points:	0 0 96 1152

# do another overlaying areadef since we're plotting by months
# but stubs are one per 3-month interval
Proc Areadef:
Saveas:	B
Xmax:	8
Ymax:	2500
Yinc:	500
Xstub.minor: 4
Xstub: 1987
	1988
	1989
	1990
	1991
	1992
	1993
	1994.

# do another overlaying areadef for the number of clinics annotation
Proc Areadef:
Xmax:	32
Ymax:	2500
Yinc:	500
Xstub.yofs: .4
Xstub.tics: none
Xstub:	.
	15
	22
	27
	29
	31
	31
	32
	32
	32
	36
	39.

Proc Text:
Position: 0.65 1.1 
Size: 8
Text: Number of Clinics--


Proc Legend:
Linetype: 0 3 8 0
Linetype.magnify: 6 6 6 6
Linethick: 0.4 0.4 0.4 1
Entry:	No. required;  Type I  = 0.01,  Type II = 0.10
	No. required;  Type I  = 0.05,  Type II = 0.10
	No. required;  Type I  = 0.05,  Type II = 0.20
	No. randomized
