#include "ConfigurationVectorVariable.h"

void GetVectorFromLine(char *line, VectorVariable *vectorCell)
{
	char *token;
	int index = 0;
	int indexSuffix = 0;
	
	while (line[index] != '{')
	{
		vectorCell->preffix[index] = line[index];
		index++;
	}
	
	index = strcspn(line, "}") + 1;
	
	while (line[index] != '\n')
	{
		vectorCell->suffix[indexSuffix] = line[index];
		index++;
		indexSuffix++;
	}
	
	token = strtok(strchr(line, '{'), "{;}'\n");
	
	while (token != NULL)
	{
		strcpy(vectorCell->values[vectorCell->numberOfValues++], token);
		
		token = strtok(NULL, "{;}'\n");
	}
}

FILE *CreateVectorStructure(VectorVariable *vectorCell)
{
	FILE *tempFile;
	int index = 0;
	
	tempFile = tmpfile();
	
	fprintf(tempFile, "%s%s%s\n", vectorCell->preffix, vectorCell->values[index++], vectorCell->suffix);
	
	while (index < vectorCell->numberOfValues)
	{
		fprintf(tempFile, ".alter\n");
		fprintf(tempFile, "%s%s%s\n", vectorCell->preffix, vectorCell->values[index++], vectorCell->suffix);
	}
    
	return tempFile;
}
