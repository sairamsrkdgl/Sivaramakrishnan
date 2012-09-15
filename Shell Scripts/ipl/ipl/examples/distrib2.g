Proc Page:
Title:	More Distribution Examples

Proc Getdata:
Data: 
0 35 9 48 
50 25 31 27 
17 14 30 17 
9 33 27 63 
0 11 50 40 
25 20 33 54 
4 4 12 19 
30 0 0 40 
75 33 67 63 
25 27 80 50 
12 23 29 22 
31 32 47 39 
29 30 25 50 
18 32 33 50 
9 35 44 44 
14 70 40 57 
21 13 25 67 
8 32 50 56 
33 36 49 21 
22 36 50 69 
0 64 6 69 
0 20 67 40 
0 36 57 33 
17 28 28 34 
13 12 23 28 
40 0 67 14 
0 30 57 36 
14 23 35 54 
0 50 50 0 
0 0 19 20 
0 0 0 50 
0 25 50 0 
0 0 0 0 
0 0 100 0 
0 0 0 0 
0 0 0 0 

Proc Areadef:
Saveas: B
Subtitle:	Fig. 5: One-way distribution without clustering 
Area.rectangle: 1 7.5 7 8.5
Xmax:		100
Xinc:		10
Xstub:		num
Ymax:		10
Yaxis:		none
Subtitle.above:	0.1

Proc Distribution:
Xfield:		2
Cluster:	n

Proc Areadef:
Clone: B
Subtitle:	Fig. 6: ..With clustering of duplicate points
Area.rectangle: 1 5.5 7 6.5

Proc Distribution:
Xfield:		2
Cluster:	y

Proc Getdata:
Data:	1 1
	1 3
	2 2
	2 2
	2 2
	4 5
	5 6
	5 6
	5 6
	5 6
	5 6
	7 3
	8 6
	8 7
	9 7
	9 7

Proc Areadef:
Saveas: A
Subtitle:	Fig. 7
		 Two-way, without clustering 
Subtitle.above: 0.3
Area:		4sw
Xmax:		10
Xinc:		2
Ymax:		10
Yinc:		2

Proc Distribution:
Xfield:		1
Yfield:		2
Mark:		sym6a
Cluster:	n

Proc Areadef:
Clone: A
Subtitle:	Fig. 8
		 ..With clustering
Area:		4se

Proc Distribution:
Xfield:		1
Yfield:		2
Mark:		sym6a
Cluster:	y
