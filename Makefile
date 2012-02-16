CC=gcc
CFLAGS=-c -Wall -Wextra
LDFLAGS=
SOURCES=main.c AnalysisMain.c AnalysisAssertions.c AnalysisLogFiles.c ConfigurationMain.c ConfigurationSteppedVariable.c ConfigurationVectorVariable.c Constants.h
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=FCAS

.SILENT:

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o
	rm FCAS 
