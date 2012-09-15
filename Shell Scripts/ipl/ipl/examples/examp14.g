
Proc Page:
Paperway:	landscape
Title:		Comparison of Monthly Enrollment with Goals
		Compatible with Several Sample Size Estimates
		== Hypertension Trial ==

Proc Getdata:
# data entered by hand from checking each monthly recruitment report
# since the beginning of the study.  Subsequent runs can just append recent
# data.
Data:	
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
Ymax:	50
Yinc:	10
Ylabel: Number of Patients Randomized
Xlabel:	Calendar Year and Month
Xstub.stubs: none
Xstub.tics:	none
Ystub.minor: 2


Proc Lineplot:
Yfield:  1
Xstart.0or1: 1

Proc Draw:
Linethick:	0.4
Linetype:	8
Linetype.magnify:  6
System: data
Saveas: A
Points:	0 12 96 12

Proc Draw:
Clone: A
Points: 0 16 96 16
Linetype: 3

Proc Draw:
Clone:	A
Points: 0 23 96 23
Linetype: 0

Proc Legend:
Linetype: 0 3 8 0
Linetype.magnify: 6 6 6 6
Linethick: 0.4 0.4 0.4 1
Entry:	No. required / month;  Type I  = 0.01,  Type II = 0.10
	No. required / month;  Type I  = 0.05,  Type II = 0.10
	No. required / month;  Type I  = 0.05,  Type II = 0.20
	No. randomized, by month

Proc Text:
Position:	6 6
Text:	P e  =  0.30,  

	( | P r - P e | / P e )  x  100%  =  25%


Proc Areadef:
Xmax:	8
Ymax:	50
Yinc:	10
Xstub.minor: 12
Xstub:	1987
	1988
	1989
	1990
	1991
	1992
	1993
	1994
	1995
	1996
