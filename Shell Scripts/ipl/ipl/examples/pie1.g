Proc Page:
Title: Pie graph example

Proc Getdata:
Data:   10
	15
	15
	10
	20
	13
	17

Proc Pie:
Field: 1
Shade: .98 .95 .9 .8 .7 .6 .5
Linethick: 2
Explode: 0.3
Rotate: 90

Proc Text:
Embeddedcoords: y
Text:
5.268     8.812     Asia
2.286     8.850     Europe
0.342     7.925     Australia
0.367     3.150     United States
2.004     2.200     Canada
5.414     2.300     Middle East
7.223     6.650     Unknown
