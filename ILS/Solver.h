#ifndef GENETIC_H
#define GENETIC_H

#include "Solution.h"
#include "ILSRVND.h"

class Solver
{
private:

	Params * params;				// Problem and method parameters
	ILSRVND localSearch;		// Local search algorithm
	void runA(Solution& finalSolution);
	void runB(Solution& finalSolution);

public:

    // Running the solver
    void run(Solution & finalSolution);

	// Running the tests
	void runTests(std::string exportFile);

	// Constructor
	Solver(Params * params);
};

#endif
