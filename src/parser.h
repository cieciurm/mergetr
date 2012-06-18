#ifndef _ANALIZATOR_H_
#define _ANALIZATOR_H_

typedef struct  
{
	char *input;             /* input file nameo */
	char *output;            /* output file name */
	char tr_opt[16];         /* Triangle options */
	int  args_start;         /* index of argv where arguments start */
} Options;

int isnumeric (char* s);

Options readParameters (int argc, char *argv[]);

int writeParameters (Options p, int argc, char **argv);

void showInfo ();

#endif
