#include "AnalysisAssertions.h"

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
