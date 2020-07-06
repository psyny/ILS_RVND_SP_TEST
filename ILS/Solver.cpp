#include "Solver.h"
#include "RVND.h"

// Routine of tests
void Solver::runTests(std::string exportFile)
{
	// Init Test Solution
	params->vehicleCapacity = 1000000000;
	Solution testSolution(params);
	testSolution.initializeTestSolution();

	// Run tests
	RVND rvnd = RVND(params);
	rvnd.runTests(testSolution, exportFile);
}

void Solver::run(Solution & finalSolution)
{
	// TODO: Decide to run algorithm A or B for ILS-RVND-SP

	//runA(finalSolution);
	runB(finalSolution);
}

void Solver::runA(Solution& finalSolution)
{

}

void Solver::runB(Solution& finalSolution)
{
	Solution bestSolution(params);
	bool bestSolutionFound = false;

	// Preparations:
	// TODO: Tolerance definition

	// Main Loop
	for (int nbIter = 0; nbIter < params->maxiter_b && clock() - params->startTime <= params->timeLimit; nbIter++)
	{
		// STEP 1: ILS-RVND
		ILSRVND ils_rvnd(params);
		Solution ILSsolution(params);
		ils_rvnd.run(ILSsolution);

		// STEP 2: SP
		// TODO: Get a solution from the SP

		// STEP 3: Update best solution and route pool
		if (ILSsolution.totalDistance < bestSolution.totalDistance || bestSolutionFound == false) {
			bestSolutionFound = true;
			bestSolution = ILSsolution;

			// TODO: add routes to route pool
		}
		
		// Tracking
		if (nbIter % ((int)params->maxiter_b / 5) == 0) std::cout << "BEST SOLUTION: " << " IT: " << nbIter << " | COST: " << bestSolution.totalDistance << std::endl;
	}

	// Copy the return
	finalSolution = bestSolution;
}

Solver::Solver(Params * params): params(params), localSearch(ILSRVND(params))
{
}
