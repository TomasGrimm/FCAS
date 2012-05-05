#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Constants.h"

#include "ConfigurationVectorVariable.h"
#include "ConfigurationSteppedVariable.h"

// function that reads the number of lines from the input file, and thus, establishes the number of structs to be created
int GetNumberOfLines(FILE *);

// main function of the variables' part of the program
// receives the input file, reads all the lines and with this information, creates the objects, the temporary structures and the output file
void TreatVariations(char *);

// this function uses the temporary files created to generate the output file
// the number of times this function is called depends on the quantity of variables
void FillOutFile(FILE *, char *, int);
