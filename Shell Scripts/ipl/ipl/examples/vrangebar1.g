Proc Page:
Title:	Vrangebar examples

Proc Getdata:
Data: 225 33 44 55 66 77 2.3 R01
      220 35 40 50 60 66 3   R02
      220 38 44 52 64 70 4.5 R03
      218 42 50 50 55 59 5   R04
      220 38 44 52 64 70 5.5 R05
      225 33 44 55 66 77 6.3 R06

Proc Areadef:
Subtitle: Figure 1: Tukey plots 
Area: 4nw
Ymin: 0
Ymax: 7 
Xmin: 0
Xmax: 100
Xinc: 10
Ystart.0or1: 1
Ystub: Northeast
	Northwest	
	Central
	Southeast
	Southwest
	West
Ystub.size: 6
Xstub.grid: line
Saveas: A

Proc Vrangebar:
Nval: 5
Field: 2 3 4 5 6
Idfield: 1
Label.position: 10

Proc Areadef:
Subtitle: Figure 2:
Clone: A
Area: 4ne

Proc Vrangebar:
Nval: 4
Field: 2 3 4 5
Ends: n
Shade: 0.9

Proc Areadef:
Subtitle: Figure 3:
Clone: A
Area: 4sw

Proc Vrangebar:
Nval: 3
Field: 2 4 6
Midlineright: y
Midlinewidth: 0.7
Shade: 0

Proc Areadef:
Subtitle: Figure 4:
Clone: A
Xstub.grid: line
Area: 4se

Proc Getdata:
Data:	1  0 30 30  80 80 100
	2  0 22 40  54 54  80
	3 13 40 40  90 90 100
	4  0 50 50 100  0   0
	5  0 70 70 100  0   0
	6  0 29 29  46 46  80 
      
Proc Vrangebar:
Nval: 2
Field: 2 3
Yfield: 1
Shade: 0.5

Proc Vrangebar:
Nval: 2
Field: 4 5
Yfield: 1
Shade: 0.9

Proc Vrangebar:
Nval: 2
Field: 6 7
Yfield: 1
Shade: 0.7 

