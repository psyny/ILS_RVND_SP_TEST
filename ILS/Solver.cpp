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
	// TODO: Decide to run algorithm A or B for ILS-RVND-SP-b

	//runA(finalSolution);
	runB(finalSolution);
}

void Solver::runA(Solution& finalSolution)
{

}

void Solver::runB(Solution& finalSolution)
{
	Solution bestSolution(params);

	// Preparations:
	// TODO: Tolerance definition

	// Main Loop
	for (int nbIter = 0; nbIter < params->maxiter_b && clock() - params->startTime <= params->timeLimit; nbIter++)
	{
		// STEP 1: ILS-RVND
		// TODO: Get a solution from the ILS-RVND

		// STEP 2: SP
		// TODO: Get a solution from the SP

		// STEP 3: Update best solution and route pool
		// TODO

		continue;

		// --------------------
		// STEP 1: Create an initial solution
		Solution mySolution(params);
		mySolution.initializeSweep();

		// STEP 2: Run the ILS

		// ILS-
		localSearch.run(mySolution);
		if (mySolution.penalizedCost < bestSolution.penalizedCost)
			bestSolution = mySolution;

		// STEP 3: Run the SP


		// Some traces
		if (nbIter % 100 == 0) std::cout << "BEST SOLUTION: " << " | IT: " << nbIter << " | DISTANCE: " << bestSolution.totalDistance << " | COST: " << bestSolution.penalizedCost << std::endl;
	}
	finalSolution = bestSolution;
}

Solver::Solver(Params * params): params(params), localSearch(ILSRVND(params))
{
}
