#include "ConfigurationSteppedVariable.h"

void GetRangeFromLine(char *line, SteppedVariable *steppedCell)
{
	char *token;
	int index = 0;
	int indexSuffix = 0;
	
	while (line[index] != '{')
	{
		steppedCell->preffix[index] = line[index];
		index++;
	}
	
	index = strcspn(line, "}") + 1;
	
	while (line[index] != '\n')
	{
		steppedCell->suffix[indexSuffix] = line[index];
		index++;
		indexSuffix++;
	}
	
	token = strtok(strchr(line, '{'), "{:}");
	steppedCell->start = atof(token);
	
	token = strtok(NULL, "{:}");
	steppedCell->end = atof(token);
	
	token = strtok(NULL, "{:}");
	steppedCell->step = atof(token);
}

FILE *CreateSteppedStructure(SteppedVariable *steppedCell)
{
	FILE *tempFile;
	double stepper = steppedCell->start;
	
	tempFile = tmpfile();
	
	fprintf(tempFile, "%s%.3f%s\n", steppedCell->preffix, stepper, steppedCell->suffix);
	stepper += steppedCell->step;
	
	while (stepper <= steppedCell->end)
	{
		fprintf(tempFile, ".alter\n");
		fprintf(tempFile, "%s%.3f%s\n", steppedCell->preffix, stepper, steppedCell->suffix);
		
		stepper += steppedCell->step;
	}
	
	return tempFile;
}
