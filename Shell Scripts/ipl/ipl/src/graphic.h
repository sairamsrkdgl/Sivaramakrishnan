#include "graphic.d"
/***** internal graphics parameters *****/
double Xlo, Xhi, Ylo, Yhi;		/* graphic area bounds, absolute coords */
double DXlo, DXhi, DYlo, DYhi;		/* graphic area bounds, data coords */
double Scale_x = 1, Scale_y = 1; 		/* linear scaling factors in x and y */
int Scale_discipline_x = LINEAR;	/* either LINEAR or LOG */
int Scale_discipline_y = LINEAR;	/* either LINEAR or LOG */
