#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "parser.h"

Options readParameters (int argc, char *argv[])
{
	int opt = 0; 
	Options wywolanie;

	wywolanie.input = "otoczka";
	wywolanie.output = "test/result";
	strcpy(wywolanie.tr_opt, "");
   
	while ( (opt = getopt(argc, argv, "p:o:a:q:")) != -1 ) 
	{
		switch (opt)                                                                   
		{
			case 'p':
         	wywolanie.input = optarg;
				if (wywolanie.input[0] == '-')
					exit(EXIT_FAILURE);
			break;

			case 'o':
				wywolanie.output = optarg;
				if (wywolanie.output[0] == '-')
					exit(EXIT_FAILURE);
			break;

			case 'a':             
				if (atof(optarg) == 0.0) {
					fprintf (stderr, "Error! 'a' option requires numeric argument.\n");
					exit(EXIT_FAILURE);
				}

				strcat(wywolanie.tr_opt, "a");
				strcat(wywolanie.tr_opt, optarg);
			break;

			case 'q':
				if (atof(optarg) == 0.0) {
					fprintf (stderr, "Error! 'q' option requires numeric argument.\n");
					exit(EXIT_FAILURE);
				}
				
				strcat(wywolanie.tr_opt, "q");
				strcat(wywolanie.tr_opt, optarg);
			break;

			default: /* '?' */
				showInfo();
				exit(EXIT_FAILURE);
		}
	}


	if (wywolanie.tr_opt[0] == '-')
		exit(EXIT_FAILURE);

	if (optind >= argc) {
		showInfo ();
		exit(EXIT_FAILURE);
	}

	wywolanie.args_start = optind;
      
	return wywolanie;
}

void showInfo ()
{
	puts("./mergetr [-p input_file][-o output_file][-q angle][-a area] mesh1 mesh2 ... meshN");
	puts("");
	puts("Options:");
	puts("");
	puts("-p\t Input file name (default 'otoczka.poly')");
	puts("-o\t Output file name (default result.node and result.ele)");
	puts("-q\t Minimal angle in a triangle");
	puts("-a\t Maximal area of a triangle");
	puts("");
	puts("Program arguments:");
	puts("");
	puts("mesh1 mesh2 ... meshN\t Mesh file names (only name core, without extension)");
}
           

int writeParameters (Options p, int argc, char **argv)
{	
	int i;

	for (i = p.args_start; i < argc; i++)
		printf("%s\n",argv[i]);

	printf("args_start= %d\n", p.args_start);
	printf("Argument opcji -p: %s\n", p.input);
	printf("Argument opcji -o: %s\n", p.output);
	printf("Argument dla Triangle'a: %s\n", p.tr_opt);
	printf("argc=%d\n", argc); 
	
	return 0;
}

