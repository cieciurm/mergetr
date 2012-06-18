#include <stdlib.h> /* NULL */
#include "triangulation.h"

int initTriangulation (struct triangulateio *out)
{
	out->pointlist = (REAL *) NULL;
	out->pointattributelist = (REAL *) NULL;
	out->pointmarkerlist = (int *) NULL;	
	out->numberofpoints = 0;				
	out->numberofpointattributes = 0;

	out->trianglelist = (int *) NULL;
	out->triangleattributelist = (REAL *) NULL;
	out->trianglearealist = (REAL *) NULL;	
	out->neighborlist = (int *) NULL;	
	out->numberoftriangles = 0;		
	out->numberofcorners = 3;	
	out->numberoftriangleattributes = 0;	

	out->segmentlist = (int *) NULL;
	out->segmentmarkerlist = (int *) NULL;
	out->numberofsegments = 0;			

	out->holelist = (REAL *) NULL;	
	out->numberofholes = 0;		
	
	return 0;
}
