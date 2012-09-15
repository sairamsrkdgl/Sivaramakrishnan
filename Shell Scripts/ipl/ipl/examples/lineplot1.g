Proc Page:
Title:	Lineplot examples

Proc Getdata:
Data:	0 0 2 -5
	3 2 4 -4.3
	6 3 6 -.8
	9 3.5 5 2.3
	12 4 5 3
	15 4.7 3.8 1.6
	18 5.4 3 1.4
	21 6 3 -0.3

Proc Areadef:
Area:	4nw
Subtitle: Fig. 1
	Basic lines w/labels
Subtitle.above: 0.3
Xmax:	27
Xinc:	3
Ymax:	10
Ymin:	-10
Yinc:	5
Saveas: A

Proc Lineplot:
Xfield: 1
Yfield: 2 3 4
Linetype: 0 1 5
Linethick: 0.5 1 2
Label:	Case 1
	Case 2
	Case 3
Saveas: B

Proc Areadef:
Clone: A
Area:	4ne
Subtitle: Fig. 2
	Stairstep option

Proc Lineplot:
Clone: B
Yfield: 3 4
Stairstep: y

Proc Areadef:
Clone: A
Area: 4sw
Subtitle: Fig. 3
	Marking data points

Proc Lineplot:
Clone: B
Yfield: 3 4
Mark:  sym1n sym3g sym6e
Mark.size: 4

Proc Areadef:
Clone: A
Area: 4se
Subtitle: Fig. 4
	Values of data points
Yticfmt: %3.1f

Proc Lineplot:
Clone: B
Stairstep: y
Yfield: 4
Numbers: y
Label: Case 3

Proc Lineplot:
Clone: B
Yfield: 3
Numbers: y
Numbers.offset: 0.05
Label:  Case 2
Mark: sym6n
Mark.size: 2

Proc Pagebreak:
Proc Page:
Title: More lineplot examples..

Proc Areadef:
Clone: A
Area: 2hi
Subtitle: Fig. 5
	Shading under lines

Proc Lineplot:
Clone: B
Yfield: 2 3 4
Shade: .98 .9 .8
Zeroat: -10

Proc Areadef:
Clone: A
Area: 2lo
Subtitle: Fig. 6
	Adjustable crossover point

Proc Getdata:
Data:
0.159     -7.286    
1.359     -6.500    
2.459     -5.500    
4.259     -2.143    
5.559     2.714     
6.809     6.500     
8.409     8.571     
10.409    8.714     
11.409    6.857     
11.609    2.500     
12.059    -2.714    
12.609    -4.214    
13.659    -1.143    
13.709    2.714     
14.409    5.500     
15.059    3.929     
15.459    -1.571    
15.859    5.643     
16.659    -3.643    
17.359    7.929     
18.259    -5.000    
19.509    9.857     
20.309    3.214     
21.159    11.143    
21.959    9.500     
22.509    6.143     
23.109    2.357     
24.409    -1.071    
25.809    -2.357    
26.759    -2.929    

Proc Lineplot:
Clone: B
Xfield: 1
Yfield: 2
Shade: 0.7
Label: Tuba
