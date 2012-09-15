# Makefile for IPL source	1-21-90 scg
#
#

IPL_OBJ = areadef.o areadress.o arrow.o bargraph.o boxplot.o \
distribution.o draw.o errorbars.o exit.o \
getdata.o initialize.o legend.o \
lineplot.o map.o nicetab.o page.o pie.o \
point.o polygon.o proc_call.o rangebar.o rect.o \
text.o vbargraph.o vector.o vrangebar.o 

GRAPHIC_OBJ = graphic.o 

PICK_OBJ = dataclick.o get_point.o pick.o siplmenu.o viplmenu.o

GDP_OBJ = main.o gget.o constraint_check.o util.o

DRIVER_OBJ = sunv.o iplps.o pcode.o

LIB_OBJ = lib.o 


# Use the following when Sunview is not available..
# LIBES = -lm 
# ipl: $(IPL_OBJ) $(GRAPHIC_OBJ) $(GDP_OBJ) $(PICK_OBJ) pcode.o iplps.o $(LIB_OBJ)
#	cc $(IPL_OBJ) $(GRAPHIC_OBJ) $(GDP_OBJ) $(PICK_OBJ) $(DRIVER_OBJ) $(LIB_OBJ) \
#	  $(LIBES) -o ../bin/ipl

# Use the following when Sunview is available..
LIBES = -lm -lsuntool -lsunwindow -lpixrect
ipl: $(IPL_OBJ) $(GRAPHIC_OBJ) $(GDP_OBJ) $(PICK_OBJ) $(DRIVER_OBJ) $(LIB_OBJ)
	cc $(IPL_OBJ) $(GRAPHIC_OBJ) $(GDP_OBJ) $(PICK_OBJ) $(DRIVER_OBJ) $(LIB_OBJ) \
	  $(LIBES) -o ../bin/ipl

$(GRPAHIC_OBJ) $(IPL_OBJ) $(GDP_OBJ) : ../install.h
$(GRAPHIC_OBJ) : graphic.h
$(GRAPHIC_OBJ) : graphic.x
$(GRAPHIC_OBJ) : graphic.d

$(IPL_OBJ) $(PICK_OBJ) : ipl.h
$(IPL_OBJ) $(PICK_OBJ) : ipl.x
$(IPL_OBJ) $(PICK_OBJ) : ipl.d
$(IPL_OBJ) $(PICK_OBJ) : graphic.h
$(IPL_OBJ) $(PICK_OBJ) : graphic.x
$(IPL_OBJ) $(PICK_OBJ) : graphic.d

$(GDP_OBJ) : gdp.h
$(GDP_OBJ) : gdp.x
$(GDP_OBJ) : gdp.d

map.o : mapdata.h
areadef.o nicetab.o : areas.h
sunv.o : shading.h
