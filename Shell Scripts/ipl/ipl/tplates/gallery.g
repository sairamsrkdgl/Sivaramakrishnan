Proc Page:
Title: IPL 1.0   Available types of plots 
Title.belowtop: 0.0

Proc Areadef:
Area.rectangle: 0.5 8 2.5 10
Subtitle: Distribution (2-way)
Xmax: 10
Xmin: 0
Ymax: 10
Ymin: 0
Xaxis: none
Yaxis: none
Frame: double
Subtitle.above: 0.08
Saveas: A

Proc Getdata:
Data: 5 5
0.922     1.562     
2.572     1.250     
2.083     2.500     
2.572     4.250     
3.428     4.813     
3.611     4.500     
5.017     4.938     
5.689     6.688     
5.811     6.063     
5.933     6.000     
7.461     5.250     
8.072     5.625     

Proc Distribution:
Xfield: 1
Yfield: 2
Mark: sym6e

Proc Getdata:
Data:
7.583     6.875     
8.439     7.938     
8.500     9.313     
5.689     3.563     
5.567     5.125     
5.200     3.937     
5.139     4.562     
4.528     3.688     
4.467     3.063     
4.467     3.625     
6.483     1.125     

Proc Distribution:
Xfield: 1
Yfield: 2
Mark: sym1n
Mark.size: 12

Proc Getdata:
Data:	2 2 FB-1
	2 7 FB-2
	2 8.1 FB-3
	2 9 FB-4

Proc Distribution:
Xfield: 1
Yfield: 2
Mark.field: 3
Mark: 

Proc Areadef:
Clone: A
Area.rectangle: 3.0 8 5.0 10
Subtitle: Lineplot

Proc Getdata:
Data: 	0.522     2.188     
	3.089     5.312     
	4.311     3.750     
	6.511     6.813     
	8.406     5.625     

Proc Lineplot:
Xfield: 1
Yfield: 2
Linetype: 1

Proc Getdata:
Data:	0.400     1.188     0.3
	2.844     2.562     2.2
	6.206     1.562     1.2
	8.344     3.312     2.4
Proc Lineplot: 
Xfield: 1
Yfield: 2
Linetype: 0
Shade: 0.9
Proc Lineplot:
Xfield: 1
Yfield: 3
Linetype: 0
Shade: 1.0

Proc Getdata:
Data:	
	0.5   6
	4  7
	7  7.5
	9  9
Proc Lineplot:
Xfield: 1
Yfield: 2
Stairstep: y

Proc Areadef:
Clone: A
Area.rectangle: 5.5 8 7.5 10
Subtitle: Bargraph

Proc Getdata:
Data: 	7
	3
	6
	2
	3
	5
	9
	4

Proc Bargraph:
Field: 1

Proc Areadef:
Clone: A
Area.rectangle: 0.5 5.5 2.5 7.5
Subtitle: Map
Xmax: 40
Ymax: 40

Proc Map:
Mode: state

Proc Areadef:
Clone: A
Area.rectangle: 3.0 5.5 5.0 7.5
Subtitle: Pie

Proc Getdata:
Data: 	12
	16
	22
	10
	17
	4
	19
	
	
Proc Pie:
Field: 1
Center: 4.0 6.5
Radius: 0.8
Shade: 0.95 0.9 0.85 0.8 0.7


Proc Areadef:
Clone: A
Area.rectangle: 5.5 5.5 7.5 7.5
Subtitle: Rangebar (error bars)
Xmax: 10
Ymax: 100

Proc Getdata:
Data: 225 33 44 55 66 77 2.3 1
      220 35 40 50 60 66 3   2
      220 38 44 52 64 70 4.5 3

Proc Rangebar:
Nval: 5
Field: 2 3 4 5 6


Proc Getdata:
Data:	
	4  15 50 50 80  0   0
	5  40 70 70 90  0   0
	6  10 29 29  46 46  80 
      
Proc Rangebar:
Nval: 2
Field: 2 3
Xfield: 1
Shade: 0.6

Proc Rangebar:
Nval: 2
Field: 4 5
Xfield: 1
Shade: 0.9

Proc Rangebar:
Nval: 2
Field: 6 7
Xfield: 1
Shade: 0.5 

Proc Getdata:
Data:	7 50 3
	8 60 4
	9 40 5

Proc Errorbars:
Xfield: 1
Yfield: 2
Errfield: 3

Proc Areadef:
Clone: A
Area.rectangle: 0.5 3.0 2.5 5.0
Subtitle: Vbargraph

Proc Getdata:
Data: 	7 6
	3 2
	6 5
	2 1
	3 2
	5 4
	9 8
	4 3

Proc Vbargraph:
Field: 1 2

Proc Areadef:
Clone: A
Area.rectangle: 3.0 3.0 5.0 5.0
Subtitle: Vector
Xmax: 100
Ymax: 100

Proc Getdata:
Data:
87.722    13.125    33.944    23.750    
73.667    8.750     21.111    11.250    
65.111    26.250    35.167    36.875    
51.667    36.250    18.667    79.375    
35.167    15.625    4.611     93.750    

Proc Vector:
Point1fields: 1 2
Point2fields: 3 4
Headlength: 0.1
Headwidth: 0.25

Proc Areadef:
Clone: A
Area.rectangle: 5.5 3.0 7.5 5.0
Subtitle: Distribution (1-way)

Proc Areadef:
Clone: A
Area.rectangle: 5.5 4.2 7.5 4.7
Subtitle: 

Proc Getdata:
Data: 	1.3
	1.6
	1.8
	2.4
	3.2
	7.6
	7.8
	1
	2
	3
	4
	5
	6
	8
	9
	
Proc Distribution:
Xfield: 1

Proc Areadef:
Clone: A
Area.rectangle: 0.5 0.5 2.5 2.5
Subtitle: Legend

Proc Legend:
Corner: A
Entry:
	symbol# sym1n
	symbol# sym2n
	symbol# sym3n
	symbol# sym4n
	symbol# sym5n
Mark:	sym1n sym2n sym3n sym4n sym5n 
Mark.size: 8

Proc Areadef:
Clone: A
Area.rectangle: 3.0 0.5 5.0 2.5
Subtitle: Vrangebar (timeline)
Xmax: 100
Ymax: 7

Proc Getdata:
Data: 225 33 44 55 66 77 2.3 1
      220 35 40 50 60 66 3   2
      220 38 44 52 64 70 4.5 3

Proc Vrangebar:
Nval: 5
Field: 2 3 4 5 6


Proc Getdata:
Data:	
	4  15 50 50 80  0   0
	5  40 70 70 90  0   0
	6  10 29 29  46 46  80 
      
Proc Vrangebar:
Nval: 2
Field: 2 3
Yfield: 1
Shade: 0.6

Proc Vrangebar:
Nval: 2
Field: 4 5
Yfield: 1
Shade: 0.9

Proc Vrangebar:
Nval: 2
Field: 6 7
Yfield: 1
Shade: 0.5 

Proc Areadef:
Clone: A
Area.rectangle: 5.5 2.2 7.5 2.5
Subtitle: Text annotation
Subtitle.above: -0.2
Subtitle.justify: center

Proc Areadef:
Clone: A
Area.rectangle: 5.5 1.9 7.5 2.2
Subtitle: Boxes
Subtitle.above: -0.2
Subtitle.justify: center

Proc Areadef:
Clone: A
Area.rectangle: 5.5 1.6 7.5 1.9
Subtitle: Arrows
Subtitle.above: -0.2
Subtitle.justify: center

Proc Areadef:
Clone: A
Area.rectangle: 5.5 1.3 7.5 1.6
Subtitle: Lines
Subtitle.above: -0.2
Subtitle.justify: center

Proc Areadef:
Clone: A
Area.rectangle: 5.5 1.0 7.5 1.3
Subtitle: Tables
Subtitle.above: -0.2
Subtitle.justify: center


Proc Areadef:
Clone: A
Area.rectangle: 5.5 0.7 7.5 1.0
Subtitle: Combinations
Subtitle.above: -0.2
Subtitle.justify: center

Proc Areadef:
Clone: A
Area.rectangle: 5.5 0.4 7.5 0.7
Subtitle: Misc
Subtitle.above: -0.2
Subtitle.justify: center

