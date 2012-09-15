Proc Page:
Paperway: landscape
Title: Predefined graphing areas-- click on your choice
Title.belowtop: 0.0

Proc Areadef:
Subtitle: whole
Subtitle.above: 0.1
Area.rectangle: 0.5 5.5 3.3 7.5
Xmax: 8.5
Ymax: 11
Xaxis: none
Yaxis: none
Frame: double
Saveas: A

# whole        
Proc Polygon:
Rectangle: 1.2 1.0 7.4 9.0
Outline: y
Shade: 0.9
System: data

Proc Areadef:
Clone: A
Area.rectangle: 4 5.5 6.8 7.5 
Subtitle: standard

# standard     
Proc Polygon:
Rectangle:  1.386     2.012     7.100     8.338     
Outline: y
Shade: 0.9
System: data

Proc Areadef:
Clone: A
Area.rectangle: 7.5 5.5 10.3 7.5
Subtitle: square

# square       
Proc Polygon:
Rectangle: 1.646     0.619     6.136     10.038    
Outline: y
Shade: 0.9
System: data


################################################

Proc Areadef:
Clone: A
Area.rectangle: 0.5 3 3.3 5
Subtitle: dist1 (for bars)

# dist1        
Proc Polygon:
Rectangle: 1.2 6.5 7.2 8.0 
Outline: y
Shade: 0.9
System: data

Proc Areadef:
Clone: A
Area.rectangle: 4.0 3 6.8 5
Subtitle: 2hi and 2lo

# 2hi          
Proc Polygon:
Rectangle: 1.0 6.0 7.6 9.5
Outline: y
Shade: 0.9
System: data

# 2lo          
Proc Polygon:
Rectangle: 1.0 1.5 7.6 5.0
Outline: y
Shade: 0.9
System: data

Proc Areadef:
Clone: A
Area.rectangle: 7.5 3 10.3 5
Subtitle: 2left and 2right

# 2left        
Proc Polygon:
Rectangle: 1.0 1.0 4.0 9.5
Outline: y
Shade: 0.9
System: data

# 2right       
Proc Polygon:
Rectangle: 5.0 1.0 8.0 9.5
Outline: y
Shade: 0.9
System: data

##################################################

Proc Areadef:
Clone: A
Area.rectangle: 0.5 0.5 3.3 2.5
Subtitle: 3hi, 3mid, 3lo

# 3hi          
Proc Polygon:
Rectangle: 1.0 7.0 7.6 9.0
Outline: y
Shade: 0.9
System: data

# 3mid         
Proc Polygon:
Rectangle: 1.0 4.0 7.6 6.0
Outline: y
Shade: 0.9
System: data

# 3lo          
Proc Polygon:
Rectangle: 1.0 1.0 7.6 3.0
Outline: y
Shade: 0.9
System: data

Proc Areadef:
Clone: A
Area.rectangle: 4.0 0.5 6.8 2.5
Subtitle: 4nw, 4ne, 4sw, 4se

# 4nw          
Proc Polygon:
Rectangle: 1.0 6.0 4.0 9.0
Outline: y
Shade: 0.9
System: data

# 4ne          
Proc Polygon:
Rectangle: 4.5 6.0 7.5 9.0
Outline: y
Shade: 0.9
System: data

# 4sw          
Proc Polygon:
Rectangle: 1.0 1.5 4.0 4.5
Outline: y
Shade: 0.9
System: data

# 4se          
Proc Polygon:
Rectangle: 4.5 1.5 7.5 4.5
Outline: y
Shade: 0.9
System: data

Proc Areadef:
Clone: A
Area.rectangle: 7.5 0.5 10.3 2.5
Subtitle: lifetab

# lifetab      
Proc Polygon:
Rectangle: 1.0 1.0 7.0 4.5
Outline: y
Shade: 0.9
System: data

Proc Areadef:
Clone: A
Area.rectangle: 9.2 7.8 10.2 8.3
Subtitle: none
Subtitle.above: -0.2
Subtitle.justify: center
Shade: 0.98
