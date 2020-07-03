#ifndef GENETIC_H
#define GENETIC_H

#include "Solution.h"
#include "LocalSearch.h"

class Solver
{
private:

	Params * params;				// Problem and method parameters
	LocalSearch localSearch;		// Local search algorithm

public:

    // Running the solver
    void run(int nbIterations, Solution & finalSolution);

	// Constructor
	Solver(Params * params);
};

#endif
