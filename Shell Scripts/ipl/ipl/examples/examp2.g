Proc Page:
Title: Logarithmic Axes

Proc Areadef:
Area: square
Xmax: 2000
Xinc: 200
Ymax: 2000
Yinc: 200
Ystub: log1
Yscaletype: log
Xstub: log1
Xscaletype: log
Shade: 0.9

Xstub.grid: line
Ystub.grid: line
Frame: double


Proc Getdata:
Data:
2.448     1.135     
3.895     3.385     
6.393     10.094    
11.876    29.628    
26.981    64.371    
55.860    115.649   
121.148   180.179   
292.822   232.134   
826.290   303.843   

Proc Lineplot:
Xfield: 1
Yfield: 2
Linethick: 3

Proc Distribution:
Xfield: 1
Yfield: 2
Mark: sym6e
Mark.size: 12

Proc Getdata:
Data:
4.478     1.533     
4.913     4.720     
5.648     27.373    
8.318     195.025   
13.032    1113.206  

Proc Lineplot:
Xfield: 1
Yfield: 2
Linethick: 3
Linetype: 1

Proc Distribution:
Xfield: 1
Yfield: 2
Mark: sym4e
Mark.size: 12

