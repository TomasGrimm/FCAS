#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Constants.h"

// struct to hold the values if they come in {start:end:step} mode
typedef struct
{
	char preffix[LineSize];	// holds the information that comes before the { sign
	double start;			// holds the first value from the range
	double end;             // holds the last value from the range
	double step;			// holds the variation step of the sequence
	char suffix[LineSize];	// holds the information that comes after the } sign
} SteppedVariable;

// this function receives a line read from the input file which contains a stepped variable
// from the line, all arguments are read and stored and the stepped fields are converted from chars to numbers
// the main fields are:
// start - the first value of the series
// end - the last value of the series
// step - the variation between each value of the series
void GetRangeFromLine(char *, SteppedVariable *);

// this function performs the creation of a temporary file that holds the structure based on a stepped variable
FILE *CreateSteppedStructure(SteppedVariable *);
