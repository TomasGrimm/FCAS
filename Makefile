CC=gcc
CFLAGS=-c -Wall -Wextra
LDFLAGS=

SOURCEFILESDIR = src
SOURCEFILES = \
	$(SOURCEFILESDIR)/main.c \
	$(SOURCEFILESDIR)/AnalysisMain.c \
	$(SOURCEFILESDIR)/AnalysisAssertions.c \
	$(SOURCEFILESDIR)/AnalysisLogFiles.c \
	$(SOURCEFILESDIR)/ConfigurationMain.c \
	$(SOURCEFILESDIR)/ConfigurationSteppedVariable.c \
	$(SOURCEFILESDIR)/ConfigurationVectorVariable.c \
	$(SOURCEFILESDIR)/Constants.h

OBJECTS=$(SOURCEFILES:.c=.o)
EXECUTABLE=FCAS

.SILENT:

all: $(SOURCEFILES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm $(SOURCEFILESDIR)/*.o
	rm FCAS
