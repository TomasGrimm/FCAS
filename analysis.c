#include "analysis.h"

void VerifySimulations(char *assertionFile)
{
	double ** values;
	char **	header;
	char file[LineSize] = "\0";
	char line[LineSize] = "\0";
	FILE *assertFile = NULL;
	FILE *simFiles = NULL;
	int i;
	int numberOfAssertions;
	
	FILE *generalLogFile = NULL;
	int totalAssertions = 0;
	int totalRight = 0;
	int totalWrong = 0;
	int perFileRight = 0;
	int perFileWrong = 0;
	
	remove(GeneralAssertsLog);
	remove(FaultsLog);
	ListFiles();
	
	numberOfAssertions = GetNumberOfAssertions(assertionFile);
	
	FILE *temporaryAssertionsLogs[numberOfAssertions];
	AssertionValues assertions[numberOfAssertions];
	
	for (i = 0; i < numberOfAssertions; i++)
	{
		assertions[i] = (AssertionValues)
		{
			.assertion = { "\0" },
			.nodeName = { "\0" },
			.xMinVal = 0.0,
			.xMaxVal = 0.0,
			.xIsRange = 0,
			.yMinVal = 0.0,
			.yMaxVal = 0.0,
			.yIsRange = 0
		};
	}
	
	if ((assertFile = fopen(assertionFile, "r")) == NULL)
		perror("Could not open assertions file");
	
	i = 0;
	
	while (fgets(line, LineSize, assertFile) != NULL)
	{
		if (strstr(line, "assert") != NULL)
		{
			temporaryAssertionsLogs[i] = tmpfile();			
			GetAssertionFromLine(line, &assertions[i]);
			
			i++;
		}
	}
	
	if (fclose(assertFile))
		perror("Could not close assertions file");
	
	if ((simFiles = fopen(SimulationFiles, "r")) == NULL)
		perror("Could not open the file listing");
	
	while ((fgets(file, LineSize, simFiles) != NULL))
	{
		if (strstr(file, ".tr") != NULL)		// makes sure that only lines with a file name are used
		{
			char *token;
			FILE *convFile;
			int currentLine = 0;
			int currentNode = 0;
			int numberOfLines = 0;
			int numberOfNodes = 0;
			int assertIndex = 0;
			
			for (i = 0; i < LineSize; i++)
				if (file[i] == '\n')				// if there is any line feed after the file name
					file[i] = '\0';				// it is exchanged for a white space
			
			ConvertFile(file);
			
			if ((convFile = fopen(ConvertedFile, "r")) == NULL)
				perror("Could not open converted file to create the map of values");
			
			GetNumberOfNodesAndLines(convFile, &numberOfNodes, &numberOfLines);

			header = (char **) malloc(numberOfNodes * sizeof(char *));
			
			if (header == NULL)
			{
				free(header);
				printf("Memory allocation failed while allocating for header[].\n");
				exit(-1);
			}
			
			/* Allocate integer memory for the second dimension of a matrix[][]; */
			for(i = 0; i < numberOfNodes; i++)
			{
				header[i] = (char *) malloc(NodeNameSize * sizeof(char));
			    if(header[i] == NULL)
				{
					free(header[i]); 
					printf("Memory allocation failed while allocating for header[i][].\n"); 
					exit(-1);
				}
			}

			/* Allocate pointer memory for the first dimension of a matrix[][]; */
			values = (double **) malloc(numberOfLines * sizeof(double *));
			if(values == NULL)
			{
				free(values); 
				printf("Memory allocation failed while allocating for values[].\n"); 
				exit(-1);
			}

			/* Allocate integer memory for the second dimension of a matrix[][]; */
			for(i = 0; i < numberOfLines; i++)
			{
				values[i] = (double *) malloc(numberOfNodes * sizeof(double));
			    if(values[i] == NULL)
				{
					free(values[i]); 
					printf("Memory allocation failed while allocating for values[i][].\n"); 
					exit(-1);
				}
			}

			// gets the names of the nodes from the simulation
			if (fgets(line, LineSize, convFile) == NULL)		// gets the first line of the simulation file
				perror("Could not read line from file");
			
			token = strtok(line, " \n");
			
			while (token != NULL)
			{
				strcpy(header[currentNode], token);
				currentNode++;
				
				token = strtok(NULL, " \n");
			}
			
			// gets the values for each node
			currentNode = 0;
			while (fgets(line, LineSize, convFile) != NULL)
			{
				token = strtok(line, " \n");
				
				while (token != NULL)
				{
					values[currentLine][currentNode] = atof(token);
					currentNode++;
					
					token = strtok(NULL, " \n");
				}
				
				currentLine++;
				currentNode = 0;
			}
			
			if (fclose(convFile))
				perror("Could not close converted file after creating the map of values");
			
			// reads the assertion file line by line and search the values' table to verify the results
			while (assertIndex < numberOfAssertions)
			{
				int nodeIndex = -1;
				int firstXnode = -1;
				int lastXnode = -1;
				
				printf("File %s ", file);		// prints the name of the file being analyzed
				
				totalAssertions++;			// counts the total number of assertions for all files
				
				nodeIndex = GetNodeIndex(assertions[assertIndex].nodeName, header, numberOfNodes);
				
				if (nodeIndex == -1)
					perror("Could not find node");
				
				// try to find the closest x-axis value to the assertion one, if it cannot find the exact value
				for (i = 0; i < numberOfLines; i++)
				{
					if ((values[i][0] == assertions[assertIndex].xMinVal) && (firstXnode == -1))		// tries to find the exact value
					{
						firstXnode = i;
					}
					else if ((values[i][0] > assertions[assertIndex].xMinVal) && (firstXnode == -1))	// tries to find the nearest value
					{
						double currentMinusPrevious, nextMinusCurrent;
						
						currentMinusPrevious = assertions[assertIndex].xMinVal - values[i - 1][0];
						nextMinusCurrent = values[i][0] - assertions[assertIndex].xMinVal;
						
						if (nextMinusCurrent < currentMinusPrevious)
							firstXnode = i;
						else if (nextMinusCurrent >= currentMinusPrevious)
							firstXnode = i - 1;
					}
					
					if (assertions[assertIndex].xIsRange)			// if the second x value is different from 0.0, it means there is a range of values to verify
					{
						if ((values[i][0] == assertions[assertIndex].xMaxVal) && (lastXnode == -1))		// tries to find the exact value
						{
							lastXnode = i;
							break;
						}
						else if ((values[i][0] > assertions[assertIndex].xMaxVal) && (lastXnode == -1))	// tries to find the nearest value
						{
							double currentMinusPrevious, nextMinusCurrent;
					
							currentMinusPrevious = assertions[assertIndex].xMaxVal - values[i - 1][0];
							nextMinusCurrent = values[i][0] - assertions[assertIndex].xMaxVal;
					
							if (nextMinusCurrent < currentMinusPrevious)
								lastXnode = i;
							else if (nextMinusCurrent >= currentMinusPrevious)
								lastXnode = i - 1;
						
							break;
						}
					}
				}
				
				if (!assertions[assertIndex].xIsRange && !assertions[assertIndex].yIsRange)
				{
					if (values[firstXnode][nodeIndex] == assertions[assertIndex].yMinVal)
					{
						totalRight++;
						perFileRight++;
					}
					else
					{
						LogFaults(temporaryAssertionsLogs[assertIndex], values[firstXnode][nodeIndex], values[firstXnode][0], &assertions[assertIndex], file);
						totalWrong++;
						perFileWrong++;
					}
				}
				else if (!assertions[assertIndex].xIsRange && assertions[assertIndex].yIsRange)
				{
					if (((values[firstXnode][nodeIndex] > assertions[assertIndex].yMinVal) && (values[firstXnode][nodeIndex] < assertions[assertIndex].yMaxVal)) || (values[firstXnode][nodeIndex] == assertions[assertIndex].yMinVal) || (values[firstXnode][nodeIndex] == assertions[assertIndex].yMaxVal))
					{
						totalRight++;
						perFileRight++;
					}
					else
					{
						LogFaults(temporaryAssertionsLogs[assertIndex], values[firstXnode][nodeIndex], values[firstXnode][0], &assertions[assertIndex], file);
						totalWrong++;
						perFileWrong++;
					}
				}
				else if (assertions[assertIndex].xIsRange && !assertions[assertIndex].yIsRange)
				{
					int hasError = 0;
					double faultPoint;
					double wrongValue;
					
					for (i = firstXnode; i <= lastXnode; i++)
					{
						if (values[i][nodeIndex] != assertions[assertIndex].yMinVal)
						{
							hasError = 1;
							faultPoint = values[i][0];
							wrongValue = values[i][nodeIndex];							
						}	
					}
					
					if (hasError)
					{
						LogFaults(temporaryAssertionsLogs[assertIndex], wrongValue, faultPoint, &assertions[assertIndex], file);
						totalWrong++;
						perFileWrong++;
					}
					else
					{
						totalRight++;
						perFileRight++;
					}
				}
				else if (assertions[assertIndex].xIsRange && assertions[assertIndex].yIsRange)
				{
					int hasError = 0;
					double faultPoint;
					double wrongValue;
					
					for (i = firstXnode; i <= lastXnode; i++)
					{
						if ((values[i][nodeIndex] < assertions[assertIndex].yMinVal) || (values[i][nodeIndex] > assertions[assertIndex].yMaxVal))
						{
							hasError = 1;
							faultPoint = values[i][0];
							wrongValue = values[i][nodeIndex];
						}
					}
					
					if (hasError)
					{
						LogFaults(temporaryAssertionsLogs[assertIndex], wrongValue, faultPoint, &assertions[assertIndex], file);
						totalWrong++;
						perFileWrong++;
					}
					else
					{
						totalRight++;
						perFileRight++;
					}
				}
				
				assertIndex++;
				
				printf("done!\n");		// prints when the current file's analysis ends
			}
		
			GenerateGeneralLog(generalLogFile, file, perFileRight, perFileWrong, numberOfAssertions);
			
			perFileRight = 0;
			perFileWrong = 0;

			for(i = 0; i < numberOfNodes; i++)
			{
				free (header[i]);
			}
			free(header);
			for(i = 0; i < numberOfLines; i++)
			{
				free (values[i]);
			}
			free(values);
		}
	}
	
	GenerateFaultsByAssertionLog(temporaryAssertionsLogs, assertions, numberOfAssertions);
	FinishGeneralLog(generalLogFile, totalAssertions, totalRight, totalWrong);
	
	if (fclose(simFiles))
		perror("Could not close the file listing");
	
	remove(SimulationFiles);		// remove auxiliary files to maintain only original
	remove(ConvertedFile);			// content from the folder
}

void GenerateGeneralLog(FILE *generalLogFile, char *file, int perFileRight, int perFileWrong, int numberOfAssertions)
{
	double percentRight, percentWrong;
	
	percentRight = (perFileRight / (double)numberOfAssertions) * 100.0;
	percentWrong = (perFileWrong / (double)numberOfAssertions) * 100.0;
	
	if ((generalLogFile = fopen(GeneralAssertsLog, "a")) == NULL)
		perror("Could not open general log");
	
	fprintf(generalLogFile, "File '%s' status:\n", file);
	fprintf(generalLogFile, "\tNumber of assertions exercised:\t%d\n", numberOfAssertions);
	fprintf(generalLogFile, "\tNumber of right assertions:\t%d\n", perFileRight);
	fprintf(generalLogFile, "\tPercent of right assertions:\t%.2f%%\n", percentRight);
	fprintf(generalLogFile, "\tNumber of wrong assertions:\t%d\n", perFileWrong);
	fprintf(generalLogFile, "\tPercent of wrong assertions:\t%.2f%%\n\n", percentWrong);
	
	if (fclose(generalLogFile))
		perror("Could not close general log");
}

void FinishGeneralLog(FILE *generalLogFile, int totalAssertions, int totalRight, int totalWrong)
{
	double percentRight, percentWrong;
	
	percentRight = (totalRight / (double)totalAssertions) * 100.0;
	percentWrong = (totalWrong / (double)totalAssertions) * 100.0;
	
	if ((generalLogFile = fopen(GeneralAssertsLog, "a")) == NULL)
		perror("Could not open general log");
	
	fprintf(generalLogFile, "Overall simulation fault coverage:\n");
	fprintf(generalLogFile, "\tTotal number of assertions:\t%d\n", totalAssertions);
	fprintf(generalLogFile, "\tNumber of right assertions:\t%d\n", totalRight);
	fprintf(generalLogFile, "\tPercent of right assertions:\t%.2f%%\n", percentRight);
	fprintf(generalLogFile, "\tNumber of wrong assertions:\t%d\n", totalWrong);
	fprintf(generalLogFile, "\tPercent of wrong assertions:\t%.2f%%\n\n", percentWrong);
	
	if (fclose(generalLogFile))
		perror("Could not close general log");	
}

void GenerateFaultsByAssertionLog(FILE *logs[], AssertionValues *assertCells, int numberOfAssertions)
{
	char buffer[LineSize];
	FILE *faultsLogFile;
	int fileSize;
	int i;
	
	if ((faultsLogFile = fopen(FaultsLog, "w")) == NULL)
		perror("Could not open faults log");
	
	for (i = 0; i < numberOfAssertions; i++)
	{
		fseek(logs[i], 0, SEEK_END);
		fileSize = ftell(logs[i]);
		rewind(logs[i]);
		
		if (fileSize > 0)
		{
			fprintf(faultsLogFile, "Assertion command: %s", assertCells[i].assertion);
			
			while (fgets(buffer, fileSize, logs[i]) != NULL)
				fputs(buffer, faultsLogFile);
		}
	}
	
	if (fclose(faultsLogFile))
		perror("Could not close faults log");
}

void LogFaults(FILE *assertFile, double actual, double faultPoint, AssertionValues *assertCell, char *dataFile)
{
	fprintf(assertFile, "In file %s:\n", dataFile);
	fprintf(assertFile, "\tAt %.20f:\n", faultPoint);
	fprintf(assertFile, "\t\tActual value:\t%.20f\n", actual);
	
	if (assertCell->yIsRange)
		fprintf(assertFile, "\t\tExpected value:\tfrom %.20f to %.20f\n\n", assertCell->yMinVal, assertCell->yMaxVal);
	else
		fprintf(assertFile, "\t\tExpected value:\t%.20f\n\n", assertCell->yMinVal);
}

int GetNodeIndex(char *nodeName, char **header, int numberOfNodes)
{
	int i;
	
	for (i = 0; i < numberOfNodes; i++)
	{
		if (!strcmp(header[i], nodeName))
			return i;
	}
	
	return -1;
}

int GetNumberOfAssertions(char *assertionFile)
{
	char line[LineSize] = "\0";
	FILE *assertFile = NULL;
	int numberOfAssertions = 0;
	
	if ((assertFile = fopen(assertionFile, "r")) == NULL)
		perror("Could not open assertions file");
		
	while (fgets(line, LineSize, assertFile) != NULL)
	{
		if (strstr(line, "assert") != NULL)
			numberOfAssertions++;
	}
	
	if (fclose(assertFile))
		perror("Could not close assertions file after reading it");
	
	return numberOfAssertions;
}

void GetAssertionFromLine(char *line, AssertionValues *assertCell)
{
	char xLine[LineSize];
	char yLine[LineSize];
	char *token;
	char *subToken;
	int i;
	
	strcpy(assertCell->assertion, line);
	
	token = strtok(line, " \n");
	
	while (token != NULL)
	{
		if (!strcmp(token, "node"))
		{
			token = strtok(NULL, " \n");
			strcpy(assertCell->nodeName, token);	// saves the node name
		}
		else if (!strcmp(token, "value"))
		{
			for (i = 0; i < LineSize; i++)
			{
				xLine[i] = '\0';
				yLine[i] = '\0';
			}

			token = strtok(NULL, "(\n");		// deals with the x values
			strcpy(xLine, token);

			token = strtok(NULL, "(\n");		// deals with the y values
			strcpy(yLine, token);
			
			if (strstr(xLine, "to") != NULL)
			{
				subToken = strtok(xLine, "( )");	// first value
				
				assertCell->xMinVal = atof(subToken);
				
				subToken = strtok(NULL, "( )");		// string "to"
				subToken = strtok(NULL, "( )");		// second value
				
				assertCell->xMaxVal = atof(subToken);
				
				assertCell->xIsRange = 1;
			}
			else
			{
				subToken = strtok(xLine, "()");		// first value
				assertCell->xMinVal = atof(subToken);
			}
			
			if (strstr(yLine, "to") != NULL)
			{
				subToken = strtok(yLine, "( )");	// first value
				
				assertCell->yMinVal = atof(subToken);
				
				subToken = strtok(NULL, "( )");		// string "to"
				subToken = strtok(NULL, "( )");		// second value
				
				assertCell->yMaxVal = atof(subToken);
				
				assertCell->yIsRange = 1;
			}
			else
			{
				subToken = strtok(yLine, "()");		// first value
				assertCell->yMinVal = atof(subToken);
			}
		}
		else
			token = strtok(NULL, " \n");
	}
}

void ListFiles()
{
	char command[CommandSize] = "\0";
	FILE *simFiles = NULL;
	
	if ((simFiles = fopen(SimulationFiles,"w")) == NULL)
		perror("Could not open file listing for writing");
	
	strcpy(command, "ls *.tr* >> ");
	strcat(command, SimulationFiles);
	
	system ("pwd");
	system(command);
	
	if (fclose(simFiles))
		perror("Could not close file listing after writing");
}

void GetNumberOfNodesAndLines(FILE *convertedFile, int *numberOfNodes, int *numberOfLines)
{
	char *token = NULL;
	char line[LineSize] = "\0";
	
	*numberOfNodes = 0;
	*numberOfLines = 0;
	
	fgets(line, LineSize, convertedFile);
	
	token = strtok(line, " ");
	
	while (token != NULL)
	{
		*numberOfNodes = *numberOfNodes + 1;
		
		token = strtok(NULL, " ");
	}
	
	while (!feof(convertedFile))
	{
		fgets(line, LineSize, convertedFile);
		
		*numberOfLines = *numberOfLines + 1;
	}
	
	rewind(convertedFile);
}

void ConvertFile(char *file)
{
	char command[CommandSize] = "\0";
	int i = 0;
	
	strcpy(command, "sp2sp -c ascii ");
	strcat(command, file);
	strcat(command, ">> ");
	strcat(command, ConvertedFile);
	
	for (i = 0; i < CommandSize; i++)
		if (command[i] == '\n')				// if there is any line feed after the file name
			command[i] = ' ';			// it is exchanged for a white space
	
	system(command);					// executes the command to convert one simulation file
}

