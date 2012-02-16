#ifndef _Constants_H_
#define _Constants_H_

// General constants
#define CommandSize         50                      // the size of the commands to be executed using the system() function
#define LineSize            1000                    // size of file line to be read
#define NodeNameSize        20                      // the maximum size a node name can have
#define ConvertedFile       "converted.txt"         // stores the results from the conversion of each simulation result file
#define SimulationFiles     "simulationFiles.txt"	// file that holds the names of the files inside the simulations results' folder

// Configuration constants
#define TemporaryFile       "temporaryFile.txt"     // holds the information read from the out file and to be rewritten afterwards

// Analysis constants
#define AssertValueSize     10                      // maximum size an assertion value can have
#define GeneralAssertsLog   "Overall.txt"           // file that contains the general coverage assertions log
#define FaultsLog           "FaultsByAssertion.txt" // file that contains the results description by assertion

#endif