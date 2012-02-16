#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Constants.h"
#include "AnalysisAssertions.h"
#include "AnalysisLogFiles.h"

// main function of the analysis' part of the tool
// this function receives as input the file containing the assertions to be made to the results of the simulation process
// it also reads these files, one by one, creates a table with the values, and apply the assertions to these values
void VerifySimulations(char *);

int GetNodeIndex(char *, char **, int);

// this function lists all files that were created as result to the simulation process
void ListFiles();

// this function counts the number of lines and nodes for each simulation file
// this information is used to generate the table that holds the converted values
void GetNumberOfNodesAndLines(FILE *, int *, int *);

void ConvertFile(char *);
