TOOLNAME=FCAS
COMPILER=gcc

.SILENT:

# Compiles all source files
all: $(TOOLNAME)

# Creates executable object
$(TOOLNAME): main.o configuration.o analysis.o
	$(COMPILER) main.o configuration.o analysis.o -o $(TOOLNAME)

# Creates main.c object file
main.o: main.c
	$(COMPILER) -c main.c -o main.o

# Creates configuration.c object file
configuration.o: configuration.c configuration.h
	$(COMPILER) -c configuration.c -o configuration.o

# Creates analysis.c object file
analysis.o: analysis.c analysis.h
	$(COMPILER) -c analysis.c -o analysis.o

# Compiles all files and runs configuration phase
run_config: all
	./$(TOOLNAME) -c $(CONFIG)

# Compiles all files and run analysis phase
run_analysis: all
	./$(TOOLNAME) -a $(ANALYZE)

# Compiles with more depth in syntax analysis
analyze:
	$(COMPILER) -Wall -Wextra main.c configuration.c analysis.c -o $(TOOLNAME)

# Cleans not needed files
clean:
	rm *.o $(TOOLNAME)

info:
	echo Available options:
	echo all
	echo run_config CONFIG=file_name
	echo run_analysis ANALYZE=file_name
	echo analyze
	echo clean
	echo info
