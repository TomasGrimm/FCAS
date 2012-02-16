#include "AnalysisLogFiles.h"

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
