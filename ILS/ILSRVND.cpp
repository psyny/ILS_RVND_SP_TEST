#include "ILSRVND.h" 
#include "RVND.h" 

// -----------------------------------------------------------------------------------

// Algorithm run
void ILSRVND::run(Solution & mySol)
{
	// Preparation 1: Estimate the number of vehicles
	// TODO

	// Main Loop
	for (int iterILS = 0; iterILS < params->maxiterILS_b; iterILS++)
	{
		// PREP 1: Generate Initial Solution
		Solution startingSolution(params);
		startingSolution.initializeSweep(); // TODO: Replace for a better way (implemented already by Bruno)

		// STEP 2: RVND Loop
		for (int iterRVND = 0; iterRVND < params->maxiterRVND_b; iterRVND++)
		{
			// STEP 2.1: Get solution from RVND
			// TODO

			// STEP 2.2: Solution Update
			// TODO

			// STEP 2.3: Perturbation 
			// TODO
		}

		// STEP 3: Solution check and update
		// TODO
	}

}




ILSRVND::ILSRVND(Params * params) : params (params)
{

}

