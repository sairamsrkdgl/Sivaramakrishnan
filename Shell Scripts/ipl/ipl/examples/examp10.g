Proc Page:
Title: Schedule Chart Example
Paperway: landscape

Proc Areadef:
Area: standard
Xmin: 9
Xmax: 18
Xstub:	9AM
	10
	11
	12~NOON
	1PM
	2
	3
	4
	5
	6PM
	7
	8
	9
Xstub.grid: line
Xstub.stubs: both
Xlabel: Monday, January 22, 1990
Ymax: 5
Ystart.0or1: 1
# These are specified from bottom up..
Ystub:  Room 1020
	Room 1044
	Room 1052
	Auditorium

Proc Getdata:
# Hours expressed in 24-hour notation, minutes in decimal
# fractions.  For example 12.5 = 12:30 pm.
Data:	1 9 11 11 12.5 12.5 15 
	2 9 10 10 12   14   17 
	3 10 14 15 18
	4 9 17
      
Proc Vrangebar:
Nval: 2
Width: .8
Field: 2 3
Yfield: 1
Shade: 0.95

Proc Vrangebar:
Nval: 2
Width: .8
Field: 4 5
Yfield: 1
Shade: 1.0

Proc Vrangebar:
Nval: 2
Width: .8
Field: 6 7
Yfield: 1
Shade: 0.9 


Proc Text:
Position:   2.15   5.20
Text: Vendor exhibits and product demonstrations                                     

Proc Text:
Position:   2.55   4.25
Text: Session 1, Intro to MIDI                                                       

Proc Text:
Position:   6.64   4.26
Text: Session 2, Intro to MIDI                                                       

Proc Text:
Position:   1.54   3.46
Text: Keynote                                                                        
Address                                                                        

Proc Text:
Position:   2.68   3.42
Text: Plenary                                                                        
Session 1                                                                      

Proc Text:
Position:   6.25   3.45
Text: Panel Discussion                                                               
System Excusive Codes                                                          

Proc Text:
Position:   1.60   2.51
Text: Current issues                                                                 
in sampling devices                                                            

Proc Text:
Position:   3.45   2.51
Text: SIG                                                                            
Luncheons                                                                      

Proc Text:
Position:   4.61   2.53
Text: Future directions                                                              
in SMTPE interfaces                                                            
