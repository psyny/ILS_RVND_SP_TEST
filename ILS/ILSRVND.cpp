#include "ILSRVND.h" 
#include "RVND.h" 
#include "NeighbStatus.h"

// -----------------------------------------------------------------------------------

// Algorithm run
void ILSRVND::runTests(Solution& mySol, std::string exportFile)
{
	NeighbStatus neighbStatus(mySol);
	RVND rvnd(params, &neighbStatus);
	rvnd.runTests(mySol, exportFile);
}

// Algorithm run
void ILSRVND::run(Solution & mySol)
{
	// Preparation 1: Estimate the number of vehicles
	// TODO

	// Preparation 2: Solutions
	Solution currentSolution(params);
	Solution innerBestSolution = currentSolution;
	Solution outerBestSolution = currentSolution;
	bool innerBestFound = false;
	bool outerBestFound = false;

	// Main Loop
	for (int restarts = 0; restarts < params->maxStartsILS_b; restarts++)
	{
		// PREP 1: Generate Initial Solution
		currentSolution.initializeSolution(); // Bruno implementation
		if (currentSolution.isFeasible == false) {
			currentSolution.initializeSweep();
		}
				
		innerBestSolution = currentSolution;

		// PREP 2: Start Support Structure to track updates
		NeighbStatus neighbStatus(currentSolution);

		// STEP 2: RVND Loop
		for (int iterILS = 0; iterILS < params->maxiterILS_b; iterILS++)
		{
			// STEP 2.1: Get solution from RVND
			RVND rvnd(params, &neighbStatus);
			rvnd.run(currentSolution);
			bool improvedOnThisStep = false;

			// STEP 2.2: Solution Update
			if (currentSolution.totalDistance < innerBestSolution.totalDistance || innerBestFound == false) {
				improvedOnThisStep = true;
				innerBestFound = true;
				innerBestSolution = currentSolution;
				iterILS = 0;
				// TODO: Vehicle update 
			}

			// STEP 2.3: Perturbation 
			rvnd.perturb(currentSolution, improvedOnThisStep);
		}

		// STEP 3: Solution check and update
		if (innerBestSolution.totalDistance < outerBestSolution.totalDistance || outerBestFound == false) {
			outerBestFound = true;
			outerBestSolution = innerBestSolution;
		}	
	}

	mySol = outerBestSolution;
}


ILSRVND::ILSRVND(Params * params) : params (params)
{

}

