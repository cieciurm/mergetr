#ifndef ADDHOLES_H
#define ADDHOLES_H

#include "mergetrTriangle.h"

typedef struct {
	int v1; 					/* first vertex of a segment */ 
	int v2;					/* second vertex of a segment */
	int is_border; 		/* if is a edge */
} EdgeList;

typedef struct {
	int no_in_mesh; 		/* "old" number in a mesh */
	int no_in_otoczka;	/* "new" number in otoczka */
	int is_border;			/* if is a point of edge */
} PointList;

EdgeList *createEdgeList (struct triangulateio siatka);

int markBndEdges (struct triangulateio siatka, EdgeList *t);

int countBndEdges (struct triangulateio siatka, EdgeList *t);

PointList *makePointList (struct triangulateio otoczka, struct triangulateio siatka, EdgeList *t);

int countBndPoints (struct triangulateio siatka, PointList *p);

int updatePoints (struct triangulateio *otoczka, struct triangulateio siatka, EdgeList *t, PointList *p);

int updateSegments (struct triangulateio *otoczka, struct triangulateio siatka, EdgeList *t, PointList *p);

int updateHoles (struct triangulateio *otoczka, struct triangulateio siatka);

int writeEdgeList (struct triangulateio siatka, EdgeList *t);

int writePointList (struct triangulateio siatka, PointList *p);

#endif 
