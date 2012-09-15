#include "graphic.d"
/***** internal graphics parameters *****/
extern double Xlo, Xhi, Ylo, Yhi;		/* graphic area bounds, absolute coords */
extern double DXlo, DXhi, DYlo, DYhi;		/* graphic area bounds, data coords */
extern double Scale_x, Scale_y; 		/* linear scaling factors in x and y */
extern int Scale_discipline_x;			/* either LINEAR or LOG */
extern int Scale_discipline_y;			/* either LINEAR or LOG */
