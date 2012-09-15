Proc Page:
Title: Areadef Terminology

Proc Areadef:
Subtitle: Dwarf Productivity for Fiscal Year 1844
Area: standard
Xmax: 6
Ymax: 50
Yinc: 10
Ystub.minor: 10
Ystub.ticlen.minor: -0.1
Xstub: 	Sleepy
	Doc
	Grumpy
	Dopey~(Absent)
	Sneezy
	
Xlabel: Dwarf Identification
Ylabel: Coal (tons/week)
Xstart.0or1: 1
Frame: double
Subtitle.above: 0.4
Ystub.grid: line



Proc Getdata:
Data: 	15
	40
	47
	08
	26

Proc Bargraph:
Field: 1
Shade: 0.9


Proc Text:
Position:   1.65   9.46
Text: Subtitle                                                                       

Proc Draw:
Points: 1.53 9.38 1.53 9.64
 2.36 9.64
 2.36 9.38
 1.53 9.38

Proc Arrow:
Points: 2.07 9.38 2.98 8.57

Proc Text:
Position:   1.67   3.07
Text: Xstubs (text)                                                                  

Proc Draw:
Points: 1.53 3.01 1.53 3.25
 2.79 3.25
 2.79 3.01
 1.53 3.01

Proc Arrow:
Points: 2.77 3.15 3.98 3.33

Proc Arrow:
Points: 2.77 3.20 3.07 3.39

Proc Text:
Position:   0.48   7.41
Text: Ystubs                                                                         
(num)                                                                          
                                                                               

Proc Draw:
Points: 0.38 7.15 0.38 7.58
 0.97 7.58
 0.97 7.15
 0.38 7.15

Proc Arrow:
Points: 0.64 7.15 0.95 5.39

Proc Arrow:
Points: 0.62 7.15 0.93 6.31

Proc Text:
Position:   3.26   2.66
Text: Xlabel                                                                         

Proc Draw:
Points: 3.15 2.60 3.15 2.80
 3.80 2.80
 3.80 2.60
 3.15 2.60

Proc Arrow:
Points: 3.76 2.74 4.28 2.89

Proc Text:
Position:   0.40   3.87
Text: Ylabel                                                                         

Proc Draw:
Points: 0.27 3.81 0.27 4.04
 0.95 4.04
 0.95 3.81
 0.27 3.81

Proc Arrow:
Points: 0.66 4.09 0.56 4.51

Proc Text:
Position:   1.19   1.57
Text: Other parameters..
  Yinc\: 10                                                                     
  Xinc\: 1                                                                      
  Xstart.0or1\: 1                                                               
  Ystub.grid\: line                                                             

Proc Draw:
Points: 0.99 0.76 0.99 1.85
 2.87 1.85
 2.87 0.76
 0.99 0.76
