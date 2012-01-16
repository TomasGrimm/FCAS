#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AssertValueSize		10			// maximum size an assertion value can have
#define CommandSize		50			// the size of the commands to be executed using the system() function
#define LineSize		1000			// size of file line to be read
#define NodeNameSize		20			// the maximum size a node name can have

#define SimulationFiles		"simulationFiles.txt"	// file that holds the names of the files inside the simulations results' folder
#define ConvertedFile		"converted.txt"		// file that stores the results from the conversion of each simulation result file
#define GeneralAssertsLog	"Overall.txt"
#define FaultsLog		"FaultsByAssertion.txt"

// struct that holds the values used to verify the simulation's results
typedef struct
{
	char assertion[LineSize];
	char nodeName[NodeNameSize];	// the name of the node to be asserted
	double xMinVal;			// the minimum x-axis value to be looked for
	double xMaxVal;			// the maximum x-axis value to be looked for, or zero if the x-axis value is discrete
	int xIsRange;
	double yMinVal;			// the minimum y-axis value to be looked for
	double yMaxVal;			// the maximum y-axis value to be looked for, or zero if the y-axis value is discrete
	int yIsRange;
	FILE *log;
} AssertionValues;

// main function of the analysis' part of the tool
// this function receives as input the file containing the assertions to be made to the results of the simulation process
// it also reads these files, one by one, creates a table with the values, and apply the assertions to these values
void VerifySimulations(char *);

int GetNodeIndex(char *, char **, int);

void GenerateGeneralLog(FILE *, char *, int, int, int);
void FinishGeneralLog(FILE *, int, int, int);
void GenerateFaultsByAssertionLog(FILE*[], AssertionValues *, int);
void LogFaults(FILE *, double, double, AssertionValues *, char *);

int GetNumberOfAssertions(char *);

// parses the received line and divides the values inside the AssertionValues struct
void GetAssertionFromLine(char *, AssertionValues *);

// this function lists all files that were created as result to the simulation process
void ListFiles();

// this function counts the number of lines and nodes for each simulation file
// this information is used to generate the table that holds the converted values
void GetNumberOfNodesAndLines(FILE *, int *, int *);

void ConvertFile(char *);
