#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mergetrIO.h"
#include "parser.h"
#include "addHoles.h"
#include "triangulation.h"
#include "glueMesh.h"

#include "mergetrTriangle.h"

int main (int argc, char **argv)
{
	Options mergetr = readParameters (argc, argv);

	char filename_mesh_node[FILENAME_MAX];
	char filename_mesh_ele[FILENAME_MAX];
	char filename_otoczka[FILENAME_MAX];
	char filename_output_node[FILENAME_MAX];
	char filename_output_ele[FILENAME_MAX];
	int no_of_meshes = argc-mergetr.args_start;

	strcpy (filename_otoczka, mergetr.input);
	if ( strstr (filename_otoczka, ".poly") == NULL) 
		strcat (filename_otoczka,  ".poly");

	strcpy (filename_output_node, mergetr.output);
	strcat (filename_output_node,  ".node");
	strcpy (filename_output_ele, mergetr.output);
	strcat (filename_output_ele,  ".ele");

	fprintf(stdout, "************************************\n");
	fprintf(stdout, "***** M * E * R * G * E * T * R ****\n");
	fprintf(stdout, "************************************\n");
	fprintf(stdout, "* Otoczka filename: %s\n", filename_otoczka);
	fprintf(stdout, "* Output filenames: %s & %s\n", filename_output_node, filename_output_ele);
	fprintf(stdout, "* Triangle options: %s\n", mergetr.tr_opt);
	fprintf(stdout, "************************************\n");

	struct triangulateio *siatka;
	struct triangulateio otoczka;
	struct triangulateio out;	
	EdgeList **v;
	PointList **p;
	int i;

	siatka = malloc ( no_of_meshes * sizeof *siatka);
	v = malloc ( no_of_meshes * sizeof **v );
	p = malloc ( no_of_meshes * sizeof **p );
		
	if (siatka == NULL || v == NULL || p == NULL) {
		fprintf (stderr, "** Error! Not enough memory!");
		return -1;
	}		

	initTriangulation (&otoczka);
	
	/* OTOCZKA */
	FILE *file_otoczka = fopen( filename_otoczka, "r"); 

	if (file_otoczka == NULL) {
		fprintf(stderr, "** Error while opening %s\n", filename_otoczka);
		return -100;
	}

	readPoints (file_otoczka, &otoczka);
	readSegments (file_otoczka, &otoczka);
	readHoles (file_otoczka, &otoczka);
	readRegions (file_otoczka, &otoczka);

	fclose (file_otoczka);

	/* MESHES */
	for (i = 0; i < (argc - mergetr.args_start); i++) {

		strcpy (filename_mesh_node, argv[mergetr.args_start+i]);
		strcat (filename_mesh_node, ".node");
		strcpy (filename_mesh_ele, argv[mergetr.args_start+i]);
		strcat (filename_mesh_ele, ".ele");

		fprintf(stdout, "************************************\n");
		fprintf(stdout, "* Mesh filenames: %s & %s\n", filename_mesh_node, filename_mesh_ele);
		
		fprintf(stdout, "************************************\n");

		FILE *file_mesh_node = fopen( filename_mesh_node, "r"); 
		FILE *file_mesh_ele = fopen( filename_mesh_ele, "r");
		
		if (file_mesh_node == NULL) {
			fprintf(stderr, "** Error while opening %s\n", filename_mesh_node);
			return -101;
		}
		if (file_mesh_node == NULL) {
			fprintf(stderr, "** Error while opening %s\n", filename_mesh_ele);
			return -102;
		}

		initTriangulation (&siatka[i]);
		readPoints (file_mesh_node, &siatka[i]);
		readTriangles (file_mesh_ele, &siatka[i]); 
	
		fclose (file_mesh_node);
		fclose (file_mesh_ele);	
	
		v[i] = createEdgeList(siatka[i]);
		markBndEdges (siatka[i], v[i]);
		p[i] = makePointList (otoczka, siatka[i], v[i]);
		
		updatePoints (&otoczka, siatka[i], v[i], p[i]);
		updateSegments (&otoczka, siatka[i], v[i], p[i]);
		updateHoles (&otoczka, siatka[i]);
	}

	fprintf(stdout, "************************************\n");

	/* TRIANGULAtE */
	initTriangulation (&out);
	strcat (mergetr.tr_opt, "pYYQ");

	triangulate (mergetr.tr_opt, &otoczka, &out, (struct triangulateio *) NULL);
	
	/* GLUE HOLES */
	/* markNotBndEdges (siatka1, v); */
	for (i = 0; i < no_of_meshes; i++) {
		glueNotBndPoints (&out, siatka[i], p[i]); /* DOKLEJANIE DO OUT */
		fixPointListNumbers (&out, &siatka[i], p[i]);
		glueInteriorTriangles (&out, siatka[i], p[i]);
		removeHole (&out);
	}	

	FILE *file_output_node = fopen (filename_output_node, "w");
	FILE *file_output_ele = fopen (filename_output_ele, "w");
	
	writePoints (file_output_node, out);
	writeTriangles (file_output_ele, out);

	fclose (file_output_node);
	fclose (file_output_ele);

	fprintf(stdout, "************************************\n");

	free (p);
	free (v);
	freeTriangulation (&otoczka);
	freeTriangulation (&out);
	for (i = 0; i < no_of_meshes; i++)
		freeTriangulation (&siatka[i]);
		
	return 0;
}
