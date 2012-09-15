# 
#
Proc Page:
  Paperway:	landscape
  Title:	CHANGE IN RESPONSE LEVEL OVER TIME
		Paradigm 17
		August 1, 1989
  Title.size:	11
  Title.belowtop:	0.7
  Standard.linethick: 0.4

# DISTRIBUTIONS
# get distribution data
Proc Getdata:
  Datafile:	data12a

# do axes (only) for distributions
# vertical is offset slightly for exact accuracy, e.g.
# so that the zero line goes down the middle
Proc Areadef:
  Saveas:	B
  Area.left:	1.0
  Area.right:	10.0
  Area.bottom:	3.48
  Area.top:	5.98
  Ylabel:	Change in Response Level
  Ylabel.size:	9
  Ymin:		-18
  Ymax:		14
  Yinc:		2
  Xmax:		13
  Xaxis:		none
  Yaxis:		both
  Ystub.tics:	both
  Ystub.stubs:	both
  
Proc Draw:
  System:		data
  Points:		0 0 13 0
  Linethick:		0.4

# set up some areas for printing headings (only)
# (so that the visit and time annotations span two graphs)
Proc Areadef:
  Saveas:	D
  Area.left:	1.6
  Area.right:	2.8
  Area.bottom: 	3.5
  Area.top:	6.0
  Xmax:		1
  Ymax: 	1
  Xaxis:	none
  Yaxis:	none
  Subtitle:	6 months
  Subtitle.justify: center
  Subtitle.size: 8
  Subtitle.above: 0.2
  
Proc Areadef:
  Clone:	D
  Area.left:	3.0
  Area.right:	4.2
  Subtitle:	12 months

Proc Areadef:
  Clone:	D
  Area.left:	4.4
  Area.right:	5.6
  Subtitle:	18 months

Proc Areadef:
  Clone:	D
  Area.left:	5.8
  Area.right:	7.0
  Subtitle:	24 months

Proc Areadef:
  Clone:	D
  Area.left:	7.2
  Area.right:	8.4
  Subtitle:	30 months

Proc Areadef:
  Clone:	D
  Area.left:	8.6
  Area.right:	9.8
  Subtitle:	36 months

# area for 1st distribution
Proc Areadef:
  Saveas:	Z
  Subtitle:	Test
  Subtitle.size: 8
  Area.left:	1.8
  Area.right:	2.4
  Area.bottom:	3.5
  Area.top:	6.0
  Ymin:		-18
  Ymax:		14
  Xmax:		20
  Xaxis:	none
  Yaxis:	none
Proc Vbargraph:
  Field:	2
  Killwild:	20
  
# area for 2nd distribution
Proc Areadef:
  Clone:	Z
  Subtitle:	Control
  Area.left:	2.4
  Area.right:	3.0
Proc Vbargraph:
  Field:	3
  Killwild:	20

# area for 3rd distribution
Proc Areadef:
  Clone:	Z
  Subtitle:	Test
  Area.left:	3.2
  Area.right:	3.8
Proc Vbargraph:
  Field:	4
  Killwild:	20

# area for 4th distribution
Proc Areadef:
  Clone:	Z
  Subtitle:	Control
  Area.left:	3.8
  Area.right:	4.4
Proc Vbargraph:
  Field:	5
  Killwild:	20

# area for 5th distribution
Proc Areadef:
  Clone:	Z
  Subtitle:	Test
  Area.left:	4.6
  Area.right:	5.2
Proc Vbargraph:
  Field:	6
  Killwild:	20

# area for 6th distribution
Proc Areadef:
  Clone:	Z
  Subtitle:	Control
  Area.left:	5.2
  Area.right:	5.8
Proc Vbargraph:
  Field:	7
  Killwild:	20

# area for 7th distribution
Proc Areadef:
  Clone:	Z
  Subtitle:	Test
  Area.left:	6.0
  Area.right:	6.6
Proc Vbargraph:
  Field:	8
  Killwild:	20

# area for 8th distribution
Proc Areadef:
  Clone:	Z
  Subtitle:	Control
  Area.left:	6.6
  Area.right:	7.2
Proc Vbargraph:
  Field:	9
  Killwild:	20

# area for 9th distribution
Proc Areadef:
  Clone: Z
  Subtitle:	Test
  Area.left:	7.4
  Area.right:	8.0
Proc Vbargraph:
  Field:	10
  Killwild:	20

# area for 10th distribution
Proc Areadef:
  Clone:	Z
  Subtitle:	Control
  Area.left:	8.0
  Area.right:	8.6
Proc Vbargraph:
  Field:	11
  Killwild:	20

# area for 11th distribution
Proc Areadef:
  Clone:	Z
  Subtitle:	Test
  Area.left:	8.8
  Area.right:	9.4
Proc Vbargraph:
  Field:	12
  Killwild:	20

# area for 12th distribution
Proc Areadef:
  Clone:	Z
  Subtitle:	Control
  Area.left:	9.4
  Area.right:	10.0
Proc Vbargraph:
  Field:	13
  Killwild:	20

# BOX PLOTS
# get data for box plots
Proc Getdata:
  Datafile:	data12b

# setup one area for all box plots
Proc Areadef:
  Clone:	B
  Area.top:	3.0
  Area.bottom:	0.5

# do the box plots
Proc Boxplot:
  Fields:	1 2 3 4 5 6
  Xloc:		1.2
  Width:	0.15
  Datarow:	1
  Saveas:	C

Proc Boxplot:
Clone:		C
Xloc:		2
Datarow:	2

Proc Boxplot:
Clone: 		C
Xloc:		3.2
Datarow:	3

Proc Boxplot:
Clone:		C
Xloc:		4
Datarow:	4

Proc Boxplot:
Clone:		C
Xloc:		5.2
Datarow:	5

Proc Boxplot:
Clone:		C
Xloc:		6
Datarow:	6

Proc Boxplot:
Clone:		C
Xloc:		7.2
Datarow:	7

Proc Boxplot:
Clone:		C
Xloc:		8
Datarow:	8

Proc Boxplot:
Clone:		C
Xloc:		9.2
Datarow:	9

Proc Boxplot:
Clone:		C
Xloc:		10
Datarow:	10

Proc Boxplot:
Clone:		C
Xloc:		11.2
Datarow:	11

Proc Boxplot:
Clone:		C
Xloc:		12
Datarow:	12

