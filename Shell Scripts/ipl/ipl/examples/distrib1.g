Proc Page: 
Title: Two-way Distribution Examples

Proc Getdata:
Data:	R01 7 9 77   11
	R02 12 10 60 21
	R03 3 8 24   31
	R04 31 29 91 41
	R05 25 25 38 51
	R06 23 24 69 41
	R07 26 24 22 61
	R08 25 27 96 71
	R09 10 8 23  81
	R10 12 11 78 91
	R11 9 31 18  81
	R12 12 13 08 71
	R13 14 12 31 81
	R14 34 30 79 71
	R15 34 36 62 61
	R16 38 38 50 100

Proc Areadef:
Saveas: A
Area:	4nw
Subtitle: Fig. 1
	Axis distribution option
Subtitle.above: 0.3
Xmax: 40
Xinc: 10
Ymax: 40
Yinc: 10
Frame: single

Proc Distribution:
Saveas: B
Xfield: 2
Yfield: 3
Cluster: n
Mark: sym6a
Axdist: y

Proc Areadef:
Clone: A
Area:	4ne
Subtitle: Fig. 2
	Observations by ID

Proc Distribution:
Clone: B
Axdist: n
Mark:
Mark.field: 1

Proc Areadef:
Clone: A
Area:  4sw
Subtitle: Fig. 3
	Size and shade from data

Proc Distribution:
Clone: B
Mark: sym6f
Sizefield: 4
Sizescale: 0.2
Shadefield: 5
Shadescale: 0.01
Axdist: n
# call again to draw lines obliterated by shading
Proc Distribution:
Clone: B
Mark: sym6a
Sizefield: 4
Sizescale: 0.2
Axdist: n

Proc Areadef:
Clone: A
Area:	4se
Subtitle: Fig. 4
	Literal Mark 

Proc Distribution:
Clone: B
Mark:  Y
Axdist: n

Proc Getdata:
Data:	R02 14 9 60
	R03 8 3 84
	R04 29 31 1
	R05 35 25 88
	R06 23 34 69
	R07 26 19 82
	R08 27 22 96
	R09 8 10 73
	R10 10 12 78
	R11 14 31 58

Proc Distribution:
Clone: B
Mark:  Z
Axdist: n
