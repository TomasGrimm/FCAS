#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ConfigurationMain.h"
#include "AnalysisMain.h"

int main (int argc, char *argv[])
{
	int argvIndex = 2;

	if ((strcmp(argv[1], "-c")) == 0)
	{
		while(argvIndex < argc)
		{
			TreatVariations(argv[argvIndex]);
			argvIndex++;
		}
	}
	else if ((strcmp(argv[1], "-a")) == 0)
	{
		while(argvIndex < argc)
		{
			VerifySimulations(argv[argvIndex]);
			argvIndex++;
		}
	}
	else if ((strcmp(argv[1], "-h")) == 0)
	{
		printf("Usage format:\n");
		printf("./FCAS [-a | -c] <input files>\n");
		printf("\t-a\tAnalisys mode - verifies the simulation output and calculates the fault coverage.\n");
		printf("\t-c\tConfiguration mode - generates the variations used in the simulation process.\n");
		printf("\t-h\tThis help.\n");
		printf("\n<input files> depend on the chosen functionality:\n");
		printf("\tfor the analysis mode, the input files are the assertions;\n");
		printf("\tfor the configuration mode, the input files are the variations.\n");
	}
	else
		printf("Invalid option.\nUse -h for help\n");
	
	return 0;
}
