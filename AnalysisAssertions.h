#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Constants.h"

#ifndef _AnalysisAssertions_H_
#define _AnalysisAssertions_H_

// struct that holds the values used to verify the simulation's results
typedef struct
{
	char assertion[LineSize];       // keeps the assertion to print in the log file
	char nodeName[NodeNameSize];	// the name of the node to be asserted
	double xMinVal;                 // the minimum x-axis value to be looked for
	double xMaxVal;                 // the maximum x-axis value to be looked for, or zero if the x-axis value is discrete
	int xIsRange;                   // specifies if x is a single value or a range of values
	double yMinVal;                 // the minimum y-axis value to be looked for
	double yMaxVal;                 // the maximum y-axis value to be looked for, or zero if the y-axis value is discrete
	int yIsRange;                   // specifies if y is a single value or a range of values
	FILE *log;                      // the file that keeps track of the current assertion's results
} AssertionValues;

// returns the number of assertions used as input to the analysis function
int GetNumberOfAssertions(char *);

// parses the received line and divides the values inside the AssertionValues struct
void GetAssertionFromLine(char *, AssertionValues *);

#endif
