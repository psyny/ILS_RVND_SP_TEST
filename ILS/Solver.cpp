#include "Solver.h"

// Routine of tests
void Solver::runTests(std::string exportFile)
{
	// Init Test Solution
	Solution testSolution(params);
	testSolution.initializeTestSolution();

	// Run tests
	localSearch.runTests(testSolution, exportFile);
}

void Solver::run(int nbIterations, Solution & finalSolution)
{
	// Example of a trivial algorithm loop which consists in repeating nbIterations of construction and local search
	// Terminates when reaching a maximum number of iterations or the time limit
	Solution bestSolution(params);

	for (int nbIter = 0;  nbIter < nbIterations && clock() - params->startTime <= params->timeLimit; nbIter++)
	{	
		// STEP 1: Create an initial solution
		Solution mySolution(params);
		mySolution.initializeSweep();

		// STEP 2: Run the ILS
		localSearch.run(mySolution);
		if (mySolution.penalizedCost < bestSolution.penalizedCost)
			bestSolution = mySolution;

		// STEP 3: Run the SP

		
		// Some traces
		if (nbIter%100 == 0) std::cout << "BEST SOLUTION: " << " | IT: " << nbIter << " | DISTANCE: " << bestSolution.totalDistance << " | COST: " << bestSolution.penalizedCost << std::endl;
	}
	finalSolution = bestSolution;
}

Solver::Solver(Params * params): params(params), localSearch(LocalSearch(params))
{}
