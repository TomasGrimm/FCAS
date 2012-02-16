#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "AnalysisAssertions.h"

void GenerateGeneralLog(FILE *, char *, int, int, int);
void FinishGeneralLog(FILE *, int, int, int);
void GenerateFaultsByAssertionLog(FILE*[], AssertionValues *, int);
void LogFaults(FILE *, double, double, AssertionValues *, char *);