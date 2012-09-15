Proc Page:
Title: Areadef examples
Standard.font: /Helvetica-Oblique

Proc Areadef:
Area: 4nw
Subtitle: Figure 1
Subtitle.above: 0.2
Ymin: 0
Ymax: 1000
Yinc: 100
Xmin: 0
Xmax: 10
Xinc: 1
Saveas: A

Proc Areadef:
Clone: A
Area: 4ne
Subtitle: Figure 2
Ystub.grid: wline
Shade: 0.95
Xmax: 18
Xstub: month
Xstub.startmonth: 10
Xstub.startyear: 86
Xstart.0or1: 1
Xstub.tics: none
Ystub.tics: none

Proc Areadef:
Clone: A
Area: 4sw
Subtitle: Figure 3
Xstub.grid: line
Xmax: 5
Xstub: Sun
	DEC
	Silicon~Graphics
	IBM
Xstart.0or1: 1
Ymin: -300
Ystub.ticlen: -0.1
Ystub.tics: both
Frame: single

Proc Areadef:
Clone: A
Area: 4se
Subtitle: Figure 4
Subtitle.above: 0.4
Ymin: 0
Ymax: 1
Yinc: 0.2
Yticfmt: %3.1f
Ystub.minor: 10
Ystub.tics: both
Ystub.stubs: both
Ystub.grid: line
Xmin: 0
Xmax: 1
Xinc: 0.2
Xstub.grid: line
Xstub.stubs: both
Xstub.tics: both
Xstub.minor: 10
Xticfmt: %3.1f

