Proc Page:
Title: More areadef examples

Proc Areadef:
Area: 4nw
Ymin: 0
Ymax: 1000
Yinc: 100
Xmin: 0
Xmax: 10
Xinc: 1
Subtitle: Figure 5
Ystub: Virginia
	Pennsylvania
	Ohio
	New Jersey
	New York
	Massachussetts
	Maryland
	Delaware
	Connecticut
Ystart.0or1: 1
Ystub.tics: none
Xstub.tics: none
Saveas: A
	
Proc Areadef:
Clone: A
Area: 4ne
Subtitle: Figure 6
Ystub.stubs: none
Xstub.tics: none
Xlabel: Standard A units
Xlabel.position: 0.3
Xlabel.size: 7
#
Proc Draw:
System: data
Points: 0 400 10 400
Linetype: 2
Linethick: 0.5
Linetype.magnify: 3
Saveas: B
Proc Draw:
Clone: B
Points: 0 500 10 500
Linetype: 3
Proc Draw:
Clone: B
Points: 0 600 10 600
Linetype: 6
#
Proc Areadef:
Clone: A
Subtitle: Figure 6
Yaxis: none
Area: 4ne
Xmin: -30
Xmax: 40
Xinc: 10
Xstub.yofs: 0.5
Xstub.tics: none
Xlabel: Standard G units
Xlabel.size: 7

Proc Areadef:
Clone: A
Area: 4sw
Subtitle: Figure 7
Ymin: -16
Ymax: 14
Yinc: 2
Ystub.size: 6
Ylabel: Change in acuity
Xmin: 0
Xmax: 30
Xinc: 3
Ystub: num
Xstub: 0
	3
	6

	12



	24
Xlabel: Time in months
Xstub.nolonetics: y

Proc Polygon:
Rectangle: 4.15 0.6 8.1 5
Shade: 0.95

Proc Areadef:
Clone: A
Subtitle: Figure 8
Area: 4se
Shade: 1.0
Xmin: 0
Xmax: 10
Xinc: 1
Ymin: 0
Ymax: 10
Yinc: 1
Ystub: num
Ystub.grid: line
Xstub.grid: line
Ystub.stubs: none
Xstub.stubs: none
Xstub.tics: none
Ystub.tics: none
