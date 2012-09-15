Proc Page:

Proc Text:
Position: 2 10.5
Text:	VIPL -- IPL Version 1.0

	  This is a utility for interactively adding 
	text annotations, lines, arrows, etc. to an 
	existing IPL control file.  These annotations
	will be added with basic, default attributes.
	You will get a chance to re-do at every step.
	Things like typesize, font, and line attributes 
	can be changed by editing the IPL control file
	that is produced.  The following are available:

	Text\: Adds 10-point text at a location pointed
	to by the mouse.  No limit to number of lines.

	Arrow\: Draws an arrow between two points given
	with the mouse.

	Line\: Draws a solid line between two points given
	with the mouse.  Line can be constrained to
	horizontal only, vertical only, or unconstrained.

	Box\: Draws a solid-line box.  The user must define
	the lower-left and the upper-right corners of the 
	box with the mouse.  If you are drawing a box around
	text, do the text first, and remember that text 
	placement is not exactly equivalent to Postscript.

	Bullet\: Places a bullet at the pointed-to location.

	Set_parms\: This is where you set things like text
	size, font, line dashing and thickness.  Values
	remain in effect until changed again.

	Datapoint\: This is a handy way to use the mouse for
	data entry, and to access the Cartesian coordinates
	of the mouse location.  The coordinates can 
	optionally be dumped into a file.  Use the left
	mouse button to enter points; pressing the right
	button sends you back to the main menu.  If a
	graphics area has been set up, coordinates will be
	in data units-- otherwise they will be in absolute
	units, i.e. inches from the lower-left corner.

	Print\: This will send a postscript image of what
	you have done so far to your laser printer.  It 
	does not save or re-save the control file.

	Redraw\: This redraws the image.

	Quit\: From here you can either 1) save the new IPL
	file under a different name; 2) re-save under the
	original file name; or 3) quit without saving.

Proc Draw:
Points: 1.45 2.55 1.45 10.69
 6.62 10.69
 6.62 2.55
 1.45 2.55
