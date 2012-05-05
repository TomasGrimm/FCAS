#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Constants.h"

// struct to hold the values if they come in vector mode {val1;val2;val3;...}
typedef struct
{
	char preffix[LineSize];				// holds the information that comes before the { sign
	char values[LineSize][CommandSize];	// vector to hold the values if they come explicitly described between the { and } signs
	char suffix[LineSize];				// holds the information that comes after the } sign
	int numberOfValues;					// counts the number of vector elements to add
} VectorVariable;

// this function receives a line read from the input file which contains a vector variable
// from the line, the arguments are read and stored, and the values and converted from chars to numbers
// a counter inside the struct keeps track of the number of values
void GetVectorFromLine(char *, VectorVariable *);

// this function performs the creation of a temporary file that holds the structure based on a vector variable
FILE *CreateVectorStructure(VectorVariable *);
