Proc Page:
Title: Vertical Bargraph examples

Proc Getdata:
Data: 
 01         0      1      0      6     10      1     11     10      2     -41
 02         3      5      0      2      6      2      3     11      3     35
 03         1      3.4    0      1      2      0      5      7      1     20
 04         2      4.8    3     11     24      9     20     11     29     -13
 05         0      3.3    1      1      4      0      8      3      2     -22
 06         2      2      0      2      7      1      6      4      4     -28
 07         1      3      0      1      6      1     24     22      3     31
 08         3      0.8    0      0      2      0      7      3      0     -15
 09         3      2.2    0      2      5      0      1      1      2     16
 10         4      8      1      6     10      1     12      2      4     48

Proc Areadef:
Area: 4nw
Subtitle: Fig. 1
	Single format, segmented:
Subtitle.above: 0.3
Xmax: 40
Xinc: 10
# The following parameter makes the first stub move over one position
Ystart.0or1: 1
# Xmax must be set to at least one greater than the number of data rows
Ymax: 11
# Use the 1st data field for X stubs
Yinc: 1
Ystub:	Model 30
	Model 40
	Model 50
	Model 55
	Model 110
	Model 112
	Model 120
	Model 220
	Model 330
	Model 440
Saveas: A

Proc Vbargraph:
Format: single
Field: 8
Shade: 0.8
Outlinebars: n


Proc Areadef:
Clone: A
Area: 4ne
Subtitle: Fig. 2
	Stack format 
Xmax: 100

Proc Vbargraph:
Format: stack
Field: 6 8 9
Shade: .5 .8 1

Proc Areadef:
Clone: A
Area.rectangle: 1.0 0.5 3.5 5
Subtitle: Fig. 3
	Cluster format

Proc Vbargraph:
Format: cluster
Field: 6 8 9
Shade: .5 .8 1

Proc Areadef:
Clone: A
Area.rectangle: 4.5 0.5 7.5 5
Subtitle: Fig. 4
	Adjustable "Zero" line
Xmax:  50
Xmin: -50
Xinc: 10

Proc Vbargraph:
Format: single
Field: 11
Shade: .5 .8 1
Idfield: 11
Zeroat: 0

Proc Draw:
Points: 0 0 0 11
System: data
