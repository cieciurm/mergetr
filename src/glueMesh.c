#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "glueMesh.h"

#define EPSILON 1e-10

int countNotBndPoints (struct triangulateio siatka, PointList *p)
{
	int no_of_points = 0;
	int i = 0;

	for (i = 0; i < siatka.numberofpoints; i++)
		if (p[i].is_border == 0)
			no_of_points++; 

	return no_of_points;
}

int countNotBndEdges (struct triangulateio siatka, EdgeList *t)
{
	int no_of_edges = 0;
	int i;

	for (i = 0; i < 3*siatka.numberoftriangles; i++) 
			if (t[i].is_border == 0)
				no_of_edges++;

	return no_of_edges;	
}

int markNotBndEdges (struct triangulateio siatka, EdgeList *t)
{
	int i, j;
	
	for (i = 0; i < 3*siatka.numberoftriangles; i++)
		if (t[i].is_border == 0)
			for (j = 0; j < 3*siatka.numberoftriangles; j++)
				if (i != j)
					if (t[i].v1 == t[j].v1 && t[i].v2 == t[j].v2)
						t[j].is_border = -1;

	return 0;
}

double distanceBetween (struct triangulateio *out, int a, struct triangulateio *siatka, int b)
{
	double distance;

	double dx = out->pointlist[2*a]   - siatka->pointlist[2*b];
	double dy = out->pointlist[2*a+1] - siatka->pointlist[2*b+1];

	distance = sqrt(dx*dx+dy*dy);

	return distance;
}

int fixPointListNumbers (struct triangulateio *out, struct triangulateio *siatka, PointList *p)
{
	int i, j;

	for (i = 0; i < siatka->numberofpoints; i++)
		for (j = 0; j < out->numberofpoints; j++)
			if ( distanceBetween (out, j, siatka, i) <= EPSILON ) {
				p[i].no_in_otoczka = j;
				break;
			}

	return 0;
}

int glueNotBndPoints (struct triangulateio *out, struct triangulateio siatka, PointList *p)
{
	int i, j;

	int new_numberofpoints = out->numberofpoints + countNotBndPoints (siatka, p);

	#ifdef DEBUG2
		fprintf (stderr, "Liczba punktów w otoczce po doklejeniu siatki wynosi: %d, a przed: %d\n", new_numberofpoints,
																															out->numberofpoints);
	#endif

	REAL *new_pointlist = realloc (out->pointlist, 2* new_numberofpoints * sizeof *new_pointlist);
	REAL *new_pointattributelist = realloc (out->pointattributelist, new_numberofpoints * out->numberofpointattributes *
														 sizeof *new_pointattributelist);

	if (new_pointlist == NULL || new_pointattributelist == NULL)
		return -170;
	else {
		out->pointlist = new_pointlist;
		out->pointattributelist = new_pointattributelist;
	}

	int noa = out->numberofpointattributes;
	int no_of_points = out->numberofpoints;

	for (i = 0; i < siatka.numberofpoints; i++) 
		if (p[i].is_border == 0) {
			out->pointlist[ 2*no_of_points ]   = siatka.pointlist [2*p[i].no_in_mesh];
			out->pointlist[ 2*no_of_points+1 ] = siatka.pointlist [2*p[i].no_in_mesh+1];
				
			for (j = 0; j < noa; j++) 
				out->pointattributelist[ noa*no_of_points + j ] = siatka.pointattributelist [ noa*p[i].no_in_mesh+ j ];
		
			no_of_points++;			
	}
	
	out->numberofpoints = new_numberofpoints;

	fprintf(stdout, "* Glued %d mesh points to triangulated polygon.\n", countNotBndPoints (siatka, p));

	return 0;
}


int glueNotBndEdges (struct triangulateio *out, struct triangulateio siatka, EdgeList *t, PointList *p)
{
	int i;

	int new_numberofsegments = out->numberofsegments + countNotBndEdges (siatka, t);
	
	#ifdef DEBUG2
		fprintf (stderr, "Liczba krawędzi w otoczce po doklejeniu siatki wynosi: %d, a przed: %d\n", new_numberofsegments,
																															out->numberofsegments);
	#endif

	int *new_segmentlist = realloc (out->segmentlist, (2 * new_numberofsegments) * sizeof out->segmentlist);
	int *new_segmentmarkerlist = realloc (out->segmentmarkerlist, new_numberofsegments * sizeof *new_segmentmarkerlist);
 
	if (new_segmentlist == NULL || new_segmentmarkerlist == NULL)
		return -80;
	else {
		out->segmentlist = new_segmentlist;
		out->segmentmarkerlist = new_segmentmarkerlist;
	}

	int no_of_segments = out->numberofsegments;

	for (i = 0; i < 3*siatka.numberoftriangles; i++) 
		if ( t[i].is_border == 0 ) {
			
			out->segmentlist[2*no_of_segments] = p[t[i].v1].no_in_otoczka+1;  
			out->segmentlist[2*no_of_segments+1] = p[t[i].v2].no_in_otoczka+1; 

			no_of_segments++;
		}

	out->numberofsegments = new_numberofsegments;
	
	printf("Glued %d mesh edges to triangulated polygon.\n", countNotBndEdges (siatka, t));
	
	return 0;
}

int glueInteriorTriangles (struct triangulateio *out, struct triangulateio siatka, PointList *p)
{
	int i;
	int new_numberoftriangles = out->numberoftriangles + siatka.numberoftriangles;

	int *new_trianglelist = realloc (out->trianglelist, 3*new_numberoftriangles * sizeof *new_trianglelist);

	#ifdef DEBUG2
		printf("Stara liczba trojkatow : %d\n\t nowa : %d\n", out->numberoftriangles, new_numberoftriangles);
	#endif
	
	if (new_trianglelist == NULL)
		return -1000;
	else
		out->trianglelist = new_trianglelist;

	int no_of_triangles = out->numberoftriangles;

	for (i = 0; i < siatka.numberoftriangles; i++, no_of_triangles++)
	{
		out->trianglelist[3*no_of_triangles]   = p[siatka.trianglelist[3*i  ]-1].no_in_otoczka+1; 
		//printf("Kopiuję do trlist[%d] : p[%d]\n", 3*no_of_triangles, p[siatka.trianglelist[3*i  ]-1].no_in_otoczka+1);
		out->trianglelist[3*no_of_triangles+1] = p[siatka.trianglelist[3*i+1]-1].no_in_otoczka+1; 
		out->trianglelist[3*no_of_triangles+2] = p[siatka.trianglelist[3*i+2]-1].no_in_otoczka+1; 
		
		
	}
	
	out->numberoftriangles = new_numberoftriangles;
	
	fprintf(stdout, "* Glued %d interior mesh triangles to triangulated polygon.\n", siatka.numberoftriangles);
	
	return 0;
}

int removeHole (struct triangulateio *out)
{
	if (out->numberofholes == 1) {
		free(out->holelist);
		out->numberofholes = 0;
		return 0;
	}

	int new_numberofholes = out->numberofholes-1;
	
	REAL *new_holelist = realloc (out->holelist, 2*new_numberofholes * sizeof *new_holelist);

	if (new_holelist == NULL)
		return -111;
	else
		out->holelist = new_holelist;

	out->numberofholes--;	

	return 0;
}
