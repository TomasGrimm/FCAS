#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LineSize		1000	// size of file line to be read
#define CommandSize		50	// the size of the commands to be executed using the system() function
#define NodeNameSize		20	// the maximum size a node name can have

#define TemporaryFile		"temporaryFile.txt"	// file that holds the information read from the out file and to be rewritten afterwards
#define SimulationFiles		"simulationFiles.txt"	// file that holds the names of the files inside the simulations results' folder
#define ConvertedFile		"converted.txt"		// file that stores the results from the conversion of each simulation result file

// struct to hold the values if they come in {start:end:step} mode
typedef struct
{
	char preffix[LineSize];		// holds the information that comes before the { sign
	double start;			// holds the first value from the range
	double end;			// holds the last value from the range
	double step;			// holds the variation step of the sequence
	char suffix[LineSize];		// holds the information that comes after the } sign
} SteppedVariable;

// struct to hold the values if they come in vector mode {val1;val2;val3;...}
typedef struct
{
	char preffix[LineSize];				// holds the information that comes before the { sign
	char values[LineSize][CommandSize];	// vector to hold the values if they come explicitly described between the { and } signs
	char suffix[LineSize];				// holds the information that comes after the } sign
	int numberOfValues;					// counts the number of vector elements to add
} VectorVariable;

// function that reads the number of lines from the input file, and thus, establishes the number of structs to be created
int GetNumberOfLines(FILE *);

// main function of the variables' part of the program
// receives the input file, reads all the lines and with this information, creates the objects, the temporary structures and the output file
void TreatVariations(char *);

// this function receives a line read from the input file which contains a stepped variable
// from the line, all arguments are read and stored and the stepped fields are converted from chars to numbers
// the main fields are:
// start - the first value of the series
// end - the last value of the series
// step - the variation between each value of the series
void GetRangeFromLine(char *, SteppedVariable *);

// this function receives a line read from the input file which contains a vector variable
// from the line, the arguments are read and stored, and the values and converted from chars to numbers
// a counter inside the struct keeps track of the number of values
void GetVectorFromLine(char *, VectorVariable *);

// this function performs the creation of a temporary file that holds the structure based on a stepped variable
FILE *CreateSteppedStructure(SteppedVariable *);

// this function performs the creation of a temporary file that holds the structure based on a vector variable
FILE *CreateVectorStructure(VectorVariable *);

// this function uses the temporary files created to generate the output file
// the number of times this function is called depends on the quantity of variables
void FillOutFile(FILE *, char *, int);
