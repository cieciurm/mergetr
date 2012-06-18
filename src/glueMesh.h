#ifndef GLUEMESH_H
#define GLUEMESH_H

#ifndef ADDHOLES_H
#include "addHoles.h"
#endif

#include "mergetrTriangle.h"

int countNotBndPoints (struct triangulateio siatka, PointList *p);

int countNotBndEdges (struct triangulateio siatka, EdgeList *t);

int markNotBndEdges (struct triangulateio siatka, EdgeList *t);

double distanceBetween (struct triangulateio *out, int a, struct triangulateio *siatka, int b); 

int fixPointListNumbers (struct triangulateio *out, struct triangulateio *otoczka, PointList *p);

int glueNotBndPoints (struct triangulateio *out, struct triangulateio siatka, PointList *p);

int glueNotBndEdges (struct triangulateio *out, struct triangulateio siatka, EdgeList *t, PointList *p);

int glueInteriorTriangles (struct triangulateio *out, struct triangulateio siatka, PointList *p);

int removeHole (struct triangulateio *out);

#endif
