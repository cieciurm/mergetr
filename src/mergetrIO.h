#ifndef _TRIANGULATION_H_
#define _TRIANGULATION_H_

#include "mergetrTriangle.h"

int readPoints (FILE *in, struct triangulateio *x); 

int readSegments (FILE *in, struct triangulateio *x);

int readHoles (FILE *in, struct triangulateio *x);

int readTriangles (FILE *in, struct triangulateio *x);

int readRegions (FILE *in, struct triangulateio *x);

int writePoints (FILE *out, struct triangulateio x);

int writeSegments (FILE *out, struct triangulateio x);

int writeTriangles (FILE *out, struct triangulateio x);

int writeHoles (FILE *out, struct triangulateio x);

void shouldWritePolyInfo (struct triangulateio in);

#endif
