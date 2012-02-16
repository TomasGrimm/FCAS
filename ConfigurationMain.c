#include "ConfigurationMain.h"

int GetNumberOfLines(FILE *fd)
{
	char line[LineSize];
	int counter = 0;
	
	while (!feof(fd))
	{
		fgets(line, LineSize, fd);
		
		counter++;
	}
	
	rewind(fd);
	
	return counter;
}

void TreatVariations(char *fileName)
{
	char line[LineSize];	// pointer to the line read from the netlist file
	FILE *inputFd;			// file descriptor, used to access its contents
	
	if ((inputFd = fopen(fileName, "r")) == NULL) 
		perror("Failed to open file!");
	
	int numberOfLines = GetNumberOfLines(inputFd);	// counts the number of comment lines to set the size of the variables's vectors
	int indexVectorVariables = 0;					// vectorVariables' iterator
	int indexSteppedVariables = 0;					// steppedVariables' iterator
	
	int tempFilesIndex = 0;
	int tempFilesIndexHelper = 0;
	FILE *tempFiles[numberOfLines];
	
	VectorVariable vectorVariables[numberOfLines];		// vector for the vector variables
	SteppedVariable steppedVariables[numberOfLines];	// vector for the stepped variables
	
	while (fgets(line, LineSize, inputFd) != NULL)
	{
		if (strstr(strchr(line, '{'), ":") != NULL)
		{
			steppedVariables[indexSteppedVariables] = (SteppedVariable)
			{
				.preffix = "\0",
				.start = 0,
				.end = 0,
				.step = 0,
				.suffix = "\0"
			};
			
			GetRangeFromLine(line, &steppedVariables[indexSteppedVariables]);
			tempFiles[tempFilesIndex++] = CreateSteppedStructure(&steppedVariables[indexSteppedVariables]);
			indexSteppedVariables++;
		}
		else
		{
			vectorVariables[indexVectorVariables] = (VectorVariable)
			{
				.preffix = "\0",
				.values = { "\0" },
				.suffix = "\0",
				.numberOfValues = 0
			};
			
			GetVectorFromLine(line, &vectorVariables[indexVectorVariables]);
			tempFiles[tempFilesIndex++] = CreateVectorStructure(&vectorVariables[indexVectorVariables]);
			indexVectorVariables++;
		}
	}
	
	if (fclose(inputFd))
		perror("Failed to close file!");
	
	int fileIsEmpty = 1;
	char outputFileName[strlen(fileName) + 10];		// sums the quantities of characters in the file name and in "_alters.sp"
	
	strcpy(outputFileName, fileName);
	strcat(outputFileName, "_alters.sp");
	
	while (tempFilesIndexHelper < tempFilesIndex)
	{
		FillOutFile(tempFiles[tempFilesIndexHelper++], outputFileName, fileIsEmpty);
		fileIsEmpty = 0;
	}
}

void FillOutFile(FILE *structureFile, char *alterFileName, int fileIsEmpty)
{
	char *buffer;
	char *alterBuffer;
	char line[LineSize];
	
	FILE *alterFile;
	FILE *tempFile;
	
	int fileSize;
	int alterFileSize;
	
	fseek(structureFile, 0, SEEK_END);
	fileSize = ftell(structureFile);
	rewind(structureFile);
	
	buffer = (char *)malloc(sizeof(char)*fileSize);
	fread(buffer, 1, fileSize, structureFile);		// now buffer contains the contents of structureFile
	
	// if the output file is still empty, just copy the first temp file's content to it
	if (fileIsEmpty)
	{
		if ((alterFile = fopen(alterFileName, "w")) == NULL)
			perror("Could not create output file");
		
		fputs(buffer, alterFile);
		
		if (fclose(alterFile))
			perror("Could not close out file after writing");
	}
	
	// otherwise, nest the next group of variable's variations
	else
	{
		// reads the contents already stored in the outFile
		if ((alterFile = fopen(alterFileName, "r")) == NULL)
			perror("Could not open output file for reading");
		
		fseek(alterFile, 0, SEEK_END);
		alterFileSize = ftell(alterFile);
		rewind(alterFile);
		
		alterBuffer = (char *)malloc(alterFileSize * sizeof(char));
		fread(alterBuffer, 1, alterFileSize, alterFile);

		if (fclose(alterFile))
			perror("Could not close out file after reading from it");
		
		// writes the content read from the outFile		
		if ((tempFile = fopen(TemporaryFile, "w")) == NULL)
			perror("Could not open temporary file for writing");
		
		fputs(alterBuffer, tempFile);
		
		if (fclose(tempFile))
			perror("Could not close temporary file after writing");
		
		// writes the content of structureFile in alterFile with the inner .alters
		if ((alterFile = fopen(alterFileName, "w")) == NULL)
			perror("Could not open outfile for writing");
		
		if ((tempFile = fopen(TemporaryFile, "r")) == NULL)
			perror("Could not open temporary file for reading");
		
		while (fgets(line, LineSize, tempFile) != NULL)
		{
			if (strstr(line, ".alter") != NULL)
				fputs(buffer, alterFile);
			
			fprintf(alterFile, "%s", line);
		}
		
		// writes the contents from structureFile at the end of the outfile for the last alteration cycle
		fputs(buffer, alterFile);
		
		if (fclose(alterFile))
			perror("Could not close out file after writing");
		
		if (fclose(tempFile))
			perror("Could not close temporary file after reading");
		
		remove(TemporaryFile);		// removes the temporary file, as it is not needed anymore

		free(alterBuffer);
	}
}
