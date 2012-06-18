#include <stdio.h>
#include <stdlib.h>

#include "addHoles.h"

EdgeList *createEdgeList (struct triangulateio siatka)
{
	EdgeList *p;
	int i;
	int a, b, c;

	p = malloc ( 3 * siatka.numberoftriangles  * sizeof *p);

	if (p == NULL)
		return NULL;
	
	for (i = 0; i < siatka.numberoftriangles; i++)
	{
		a = siatka.trianglelist[3*i]-1; /* nodes numbers from 0 instead of 1 */
		b = siatka.trianglelist[3*i+1]-1;
		c = siatka.trianglelist[3*i+2]-1;

		if (a < b) {
			p[3*i].v1 = a;
			p[3*i].v2 = b;	
		} else {
			p[3*i].v1 = b;
			p[3*i].v2 = a;	
		}

		if (a < c) {
			p[3*i+1].v1 = a;
			p[3*i+1].v2 = c;	
		} else {
			p[3*i+1].v1 = c;
			p[3*i+1].v2 = a;	
		}
	
		if (c < b) {
			p[3*i+2].v1 = c;
			p[3*i+2].v2 = b;	
		} else {
			p[3*i+2].v1 = b;
			p[3*i+2].v2 = c;	
		}			
		
	}	

	for (i = 0; i < siatka.numberoftriangles; i++)
		p[i].is_border = -1;
	
	return p;
}

int markBndEdges (struct triangulateio siatka, EdgeList *t)
{
	int i, j;
	int token = 0;

	for (i = 0; i < 3*siatka.numberoftriangles; i++)
	{
		for (j = 0; j < 3*siatka.numberoftriangles; j++)
		 	if (i != j)
				if (t[i].v1 == t[j].v1 && t[i].v2 == t[j].v2) 
					token = 1;
	
		if (token == 1)
			t[i].is_border = 0;
		else 
			t[i].is_border = 1;
				
		token = 0;
	}

	return 0;
}

int countBndEdges (struct triangulateio siatka, EdgeList *t)
{
	int no_of_edges = 0;
	int i;

	for (i = 0; i < 3*siatka.numberoftriangles; i++) 
			if (t[i].is_border == 1)
				no_of_edges++;
	
	return no_of_edges;
}

int countBndPoints (struct triangulateio siatka, PointList *p)
{
	int no_of_points = 0;
	int i = 0;

	for (i = 0; i < siatka.numberofpoints; i++)
		if (p[i].is_border == 1)
			no_of_points++; 

	return no_of_points;
}

PointList *makePointList (struct triangulateio otoczka, struct triangulateio siatka, EdgeList *t)
{
	int i, j;

	PointList *p = malloc ( siatka.numberofpoints * sizeof *p);

	if (p == NULL)
		return NULL;
	
	for (i = 0; i < siatka.numberofpoints; i++) {
		p[i].no_in_mesh = i;
		
		for (j = 0; j < 3*siatka.numberoftriangles; j++)
			if ( (p[i].no_in_mesh == t[j].v1 || p[i].no_in_mesh == t[j].v2) && t[j].is_border == 1 )
				p[i].is_border = 1;
	}
	
	for (i = 0; i < siatka.numberofpoints; i++) /* Marking boundary points */
		if ( p[i].is_border == 1 )
			p[i].no_in_otoczka = otoczka.numberofpoints++;

	
	for (i = 0; i < siatka.numberofpoints; i++) /* Marking notboundary points */
		if ( p[i].is_border == 0 )
			p[i].no_in_otoczka = otoczka.numberofpoints++;
	
	return p;
}

int updatePoints (struct triangulateio *otoczka, struct triangulateio siatka, EdgeList *t, PointList *p)
{
	int i, j;

	if (otoczka->numberofpointattributes != siatka.numberofpointattributes) {
		printf("Error! Otoczka and mesh have different number of point attributes.\n");
		return -71;
	}

	int new_numberofpoints = otoczka->numberofpoints + countBndPoints (siatka, p);

	#ifdef DEBUG
	fprintf (stderr, "Nowa liczba punktów w otoczce wynosi: %d\n", new_numberofpoints);
	#endif

	REAL *new_pointlist = realloc (otoczka->pointlist, 2* new_numberofpoints * sizeof *new_pointlist);
	REAL *new_pointattributelist = realloc (otoczka->pointattributelist, new_numberofpoints * otoczka->numberofpointattributes *
														 sizeof *new_pointattributelist);

	if (new_pointlist == NULL || new_pointattributelist == NULL)
		return -70;
	else {
		otoczka->pointlist = new_pointlist;
		otoczka->pointattributelist = new_pointattributelist;
	}

	int noa = otoczka->numberofpointattributes;
	int no_of_points = otoczka->numberofpoints;

	#ifdef ADDHOLEDBG
		printf("\nRozmiar otoczka->pointlist: %d\n", 2*new_numberofpoints);
	#endif

	for (i = 0; i < siatka.numberofpoints; i++) 
		if (p[i].is_border == 1) {
			otoczka->pointlist[ 2*no_of_points ]   = siatka.pointlist [2*p[i].no_in_mesh];
			otoczka->pointlist[ 2*no_of_points+1 ] = siatka.pointlist [2*p[i].no_in_mesh+1];
				
			#ifdef ADDHOLEDBG
				printf("i = %d. Dobieram sie do otoczka->pointlist[%d] i [%d]\n", i, 2*no_of_points, 2*no_of_points+1);
				printf("\t\tLaduje tam siatka.pointlist[%d] i [%d]\n", 2*p[i].no_in_mesh, 2*p[i].no_in_mesh+1);
			#endif				
			
			for (j = 0; j < noa; j++) 
				otoczka->pointattributelist[ noa*no_of_points + j ] = siatka.pointattributelist [ noa*p[i].no_in_mesh+ j ];
		
			no_of_points++;			
	}
	
	otoczka->numberofpoints = new_numberofpoints;

	fprintf(stdout, "* Added %d boundary mesh points.\n", countBndPoints(siatka, p)); 
	
	return 0;
}

int updateSegments (struct triangulateio *otoczka, struct triangulateio siatka, EdgeList *t, PointList *p)
{
	int i;

	int new_numberofsegments = otoczka->numberofsegments + countBndEdges (siatka, t);
	
	int *new_segmentlist = realloc (otoczka->segmentlist, (2 * new_numberofsegments) * sizeof otoczka->segmentlist);
	int *new_segmentmarkerlist = realloc (otoczka->segmentmarkerlist, new_numberofsegments * sizeof *new_segmentmarkerlist);
 
	if (new_segmentlist == NULL || new_segmentmarkerlist == NULL)
		return -80;
	else {
		otoczka->segmentlist = new_segmentlist;
		otoczka->segmentmarkerlist = new_segmentmarkerlist;
	}

	int no_of_segments = otoczka->numberofsegments;

	for (i = 0; i < 3*siatka.numberoftriangles; i++) 
		if ( t[i].is_border == 1 ) {
			
			otoczka->segmentlist[2*no_of_segments] = p[t[i].v1].no_in_otoczka+1;  
			otoczka->segmentlist[2*no_of_segments+1] = p[t[i].v2].no_in_otoczka+1; 

			#ifdef POINTSDB
			#endif

			no_of_segments++;
		}
	
	if (otoczka->segmentmarkerlist != NULL)	
		for (i = otoczka->numberofsegments; i < new_numberofsegments; i++)
			otoczka->segmentmarkerlist[i] = 0;

	otoczka->numberofsegments = new_numberofsegments;
	
	fprintf(stdout, "* Added %d boundary mesh edges.\n", countBndEdges(siatka, t)); 
	
	return 0;
}

int updateHoles (struct triangulateio *otoczka, struct triangulateio siatka)
{
	REAL ox, oy; 
	
	REAL x1 = siatka.pointlist[2*siatka.trianglelist[0]-2];
	REAL y1 = siatka.pointlist[2*siatka.trianglelist[0]-1];
	REAL x2 = siatka.pointlist[2*siatka.trianglelist[1]-2];
	REAL y2 = siatka.pointlist[2*siatka.trianglelist[1]-1];
	REAL x3 = siatka.pointlist[2*siatka.trianglelist[2]-2];
	REAL y3 = siatka.pointlist[2*siatka.trianglelist[2]-1];

	int new_numberofholes = otoczka->numberofholes+1;

	REAL *new_holelist = realloc (otoczka->holelist, 2*new_numberofholes * sizeof *new_holelist);

	if (new_holelist == NULL)
		return -90;
	else
		otoczka->holelist = new_holelist;
	
	ox = (x1 + x2 + x3)/3;
	oy = (y1 + y2 + y3)/3;

	otoczka->holelist[2*otoczka->numberofholes] = ox;
	otoczka->holelist[2*otoczka->numberofholes+1] = oy;
	
	otoczka->numberofholes++;

	return 0;
}

int writeEdgeList (struct triangulateio siatka, EdgeList *t)
{
	int i;
	
	printf("\nList of edges:\n");
	printf("| v1 | v2 | b |\n");

	for (i=0; i < 3*siatka.numberoftriangles; i++)
		printf("  %d    %d   %d\n", t[i].v1, t[i].v2, t[i].is_border);
		
	return 0;
}

int writePointList (struct triangulateio siatka, PointList *p)
{
	int i;

	printf("\nList of points:\n");
	printf("| # | # | b |\n");	

	for (i = 0; i < siatka.numberofpoints; i++)
		printf("  %d    %d   %d\n", p[i].no_in_mesh, p[i].no_in_otoczka, p[i].is_border);

	return 0;
}
