#include <stdio.h>
#include <stdlib.h>

#include "mergetrIO.h"

int readPoints (FILE *in, struct triangulateio *x) 
{
	int tmp, tmp_markers;
	int i, j;

	if (fscanf(in, "%d %d %d %d", &x->numberofpoints, &tmp, &x->numberofpointattributes, &tmp_markers) != 4)
		return -1;

	if (x->numberofpoints > 0)
		x->pointlist = malloc (2 * x->numberofpoints * sizeof *x->pointlist);
	else
		return -2;

	if (x->numberofpointattributes > 0)
		x->pointattributelist = malloc ( x->numberofpoints * x->numberofpointattributes * sizeof *x->pointattributelist);
	else
		x->pointattributelist = NULL;
	
	if (tmp_markers == 1)
		x->pointmarkerlist = malloc (x->numberofpoints * sizeof *x->pointmarkerlist);
	else if (tmp_markers == 0)
		x->pointmarkerlist = NULL;
	else
		return -3;
	
	for (i = 0; i < x->numberofpoints; i++) {
	
		if ( fscanf(in, "%d %g %g", &tmp, &x->pointlist[2*i], &x->pointlist[2*i+1]) != 3 )
			return -5;

		if (x->numberofpointattributes != 0)
			for (j = i*x->numberofpointattributes; j < i*x->numberofpointattributes+x->numberofpointattributes; j++)
				fscanf (in, "%g", &x->pointattributelist[j]);
		
		if (x->pointmarkerlist != NULL)
			if (fscanf (in, "%d", &x->pointmarkerlist[i]) != 1)
				return -6;
		}
	
	fprintf(stdout, "* Read %d points with %d attribute(s).\n", x->numberofpoints, x->numberofpointattributes);	 

	return 0;
}

int readSegments (FILE *in, struct triangulateio *x)
{
	int tmp, tmp_markers;
	int i;

	if (fscanf(in, "%d %d", &x->numberofsegments, &tmp_markers) != 2)
		return -7;

	if (x->numberofsegments > 0)
		x->segmentlist = malloc (2 * x->numberofsegments * sizeof *x->segmentlist);
	else
		return -8;

	if (tmp_markers == 1)
		x->segmentmarkerlist = malloc (x->numberofsegments * sizeof *x->segmentmarkerlist);
	else if (tmp_markers == 0)
		x->segmentmarkerlist = NULL;
	else 
		return -9;

	x->segmentmarkerlist = NULL;

	for (i = 0; i<x->numberofsegments; i++)
	{
		if (fscanf (in, "%d %d %d", &tmp, &x->segmentlist[2*i], &x->segmentlist[2*i+1]) != 3)
			return -10;
	
		if (x->segmentmarkerlist != NULL)
			if (fscanf (in, "%d", &x->segmentmarkerlist[i]) != 1)
				return -11; 
	}

	if (x->segmentmarkerlist == NULL)
		fprintf(stdout, "* Read %d segments without boundary markers.\n", x->numberofsegments);	
	else
		fprintf(stdout, "* Read %d segments wit boundary markers.\n", x->numberofsegments);	
	
	return 0;
}	

int readHoles (FILE *in, struct triangulateio *x)
{
	int tmp;
	int i;

	if (fscanf (in, "%d", &x->numberofholes) == 1)
		x->holelist = malloc (2 * x->numberofholes * sizeof *x->holelist);
	else if (fscanf (in, "%d", &x->numberofholes) == 0)
		x->holelist = NULL;  
	
	if (x->holelist != NULL)
		for (i = 0; i < x->numberofholes; i++)
			if (fscanf (in, "%d %g %g", &tmp, &x->holelist[2*i], &x->holelist[2*i+1]) != 3)
				return -12;
	
	fprintf(stdout, "* Read %d hole(s).\n", x->numberofholes);
	
	return 0;
}

int readTriangles (FILE *in, struct triangulateio *x)
{
	int tmp;
	int i, j;

	if (fscanf (in, "%d %d %d", &x->numberoftriangles, &tmp, &x->numberoftriangleattributes) != 3)
		return -13;

	if (x->numberoftriangles > 0)
		x->trianglelist = malloc (3 * x->numberoftriangles * sizeof *x->trianglelist);
	else
		return -14;

	if (x->numberoftriangleattributes > 0)
		x->triangleattributelist = malloc (2 * x->numberoftriangles * x->numberoftriangleattributes * sizeof *x->triangleattributelist);
	else if (x->numberoftriangleattributes == 0)
		x->triangleattributelist = NULL;
	else
		return -14;
	
	if (tmp != 3) {
		fprintf(stderr, "** Error, while reading *.ele file! Nodes per triangle must be 3.\n");
		return -15;
	}

	for (i = 0; i < x->numberoftriangles; i++) {
	
		if ( fscanf(in, "%d %d %d %d", &tmp, &x->trianglelist[3*i], &x->trianglelist[3*i+1], &x->trianglelist[3*i+2]) != 4)
			return -16;

		if (x->numberoftriangleattributes > 0)
			for (j = i*x->numberoftriangleattributes; j < i*x->numberoftriangleattributes+x->numberoftriangleattributes; j++)
				if (fscanf (in, "%g", &x->triangleattributelist[j]) != 1)
					return -17;
		}

	fprintf(stdout, "* Read %d triangles with %d attribute(s).\n", x->numberoftriangles, x->numberoftriangleattributes);
	
	return 0;
}

int readRegions (FILE *in, struct triangulateio *x)
{
	//if (fscanf(in, "%d", &x->numberofregions) != 1)
		x->numberofregions = 0;

	return 0;
}

int writePoints (FILE *out, struct triangulateio x)
{
	int tmp_markers;
	int i,j;

	if (x.pointmarkerlist == NULL)
		tmp_markers = 0;
	else
		tmp_markers = 1;

	fprintf( out, "%d 2 %d %d\n", x.numberofpoints, x.numberofpointattributes, tmp_markers);
	
	for (i = 0; i < x.numberofpoints; i++)
	{
		fprintf (out, "%d %g %g", i+1, x.pointlist[2*i], x.pointlist[2*i+1]); 
			
		if (x.numberofpointattributes > 0)
			for (j = i*x.numberofpointattributes; j < i*x.numberofpointattributes+x.numberofpointattributes; j++)
				fprintf (out, " %f", x.pointattributelist[j]);
		
		if (x.pointmarkerlist != NULL)
			fprintf (out, " %d", x.pointmarkerlist[i]);
		
		fprintf(out, "\n");
	}
			
	return 0;
}	

int writeSegments (FILE *out, struct triangulateio x)
{
	int i;
	int tmp; 
	
	if (x.segmentmarkerlist == NULL)
		tmp = 0;
	else
		tmp = 1;
	
	fprintf(out, "%d %d\n", x.numberofsegments, tmp);

	for (i = 0; i < x.numberofsegments; i++) {
		fprintf(out, "%d %d %d", i+1, x.segmentlist[2*i], x.segmentlist[2*i+1]);
		
		if (tmp == 1)
			fprintf(out, " %d", x.segmentmarkerlist[i]);
		
		fprintf(out, "\n");
	}
	return 0;
}

int writeTriangles (FILE *out, struct triangulateio x)
{
	int i, j;

	fprintf (out, "%d 3 %d\n", x.numberoftriangles, x.numberoftriangleattributes);

	for (i = 0; i < x.numberoftriangles; i++)
	{
		fprintf (out, "%d %d %d %d", i+1, x.trianglelist[3*i], x.trianglelist[3*i+1], x.trianglelist[3*i+2]);
			
		if (x.numberoftriangleattributes > 0)
			for (j = i*x.numberoftriangleattributes; j < i*x.numberoftriangleattributes+x.numberoftriangleattributes; j++)
				fprintf (out, " %g", x.triangleattributelist[j]);
		
		fprintf(out, "\n");
	}
	
	return 0;
}

int writeHoles (FILE *out, struct triangulateio x)
{
	int i;
	
	fprintf(out, "%d\n", x.numberofholes);

	for (i = 0; i < x.numberofholes; i++)
		fprintf(out, "%d %.3g %.3g\n", i+1, x.holelist[2*i], x.holelist[2*i+1]);

	return 0;
}

void shouldWritePolyInfo (struct triangulateio in)
{
	int i;
	
	puts("\n\n---INFO----");
	printf("Points: %d\n", in.numberofpoints);
	printf("Attributes: %d\n", in.numberofpointattributes);
	if (in.pointmarkerlist == NULL)
		printf("Markers: OFF\n");
	else 
		printf("Markers: ON\n");
	for (i = 0; i < in.numberofpoints; i++)
			if (in.numberofpointattributes == 0)
				printf("Vertex: %f %f\n", in.pointlist[2*i], in.pointlist[2*i+1]);
			else 
				printf("Vertex: %f %f %f\n", in.pointlist[2*i], in.pointlist[2*i+1], in.pointattributelist[i]);

	printf("Segments: %d\n", in.numberofsegments);
	printf("Holes: %d\n", in.numberofholes);
	puts("-----------");
}
