Proc Page:
Title:	Error bar example

Proc Areadef:
Area.rectangle:	2 5 6 8
Ymax:	0.25
Yinc:	0.05
Ymin:  -0.10
Xmax:	0.20
Xmin:	-0.05
Xinc:	0.05
Xticfmt: %4.2f
Yticfmt: %4.2f
Ylabel:	95% CI on p1 - p2
Xlabel:	Observed Difference in Mortality ( p1 - p2 )

Proc Getdata:
Data:	0	.080 .057 .046 .040 .037
	0.05    .078 .055 .045 .039 .036
	0.1     .075 .053 .043 .038 .034
	0.15    .072 .051 .041 .036 .033

Proc Lineplot:
Xfield:	1
Yfield:	1
	
Proc Errorbars:
Xfield: 1
Yfield:	1
Errfield: 2
Taillen: 0.1
Double:	n
Saveas:	A

Proc Errorbars:
Clone:	A
Errfield: 3

Proc Errorbars:
Clone:	A
Errfield: 4

Proc Errorbars:
Clone:	A
Errfield:  5

Proc Errorbars:
Clone: A
Errfield:  6

Proc Text:
System:	absolute
Position:  5.5 8
Text:	N = 500
	N = 1000
	N = 1500
	N = 2000
	N = 2400
