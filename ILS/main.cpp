#include "Solver.h"
#include "commandline.h"
#include "LocalSearch.h"
using namespace std;

int main(int argc, char *argv[])
{
	commandline c(argc, argv);
	if (c.is_valid())
	{
		// Read the instance and calculate distance matrix
		Params params(c.get_path_to_instance(), c.get_seed());
		std::cout << "----- INSTANCE LOADED: " << c.get_path_to_instance() << " WITH " << params.nbClients << " CUSTOMERS AND " << params.nbVehicles << " VEHICLES -----" << std::endl;

		if (c.is_test()) {
			// Test Process 01
			std::cout << "----- TEST PROCEDURES -----" << std::endl;
			Solver testSolver(&params);
			testSolver.runTests(c.get_path_to_solution());
		} 
		else
		{
			// Solution process
			std::cout << "----- START OF THE ALGORITHM -----" << std::endl;
			params.startTime = clock();
			params.timeLimit = c.get_cpu_time() * CLOCKS_PER_SEC;
			Solution mySolution(&params);				// Creating a solution object
			Solver mySolver(&params);					// Creating a solver object
			mySolver.run(5000, mySolution);				// Running the solver for 5000 iterations (repeated construction and local search)
			params.endTime = clock();
			std::cout << "----- END OF THE ALGORITHM -----" << std::endl;

			// Exporting the best solution
			mySolution.exportToFile(c.get_path_to_solution());
		}
	}
	return 0;
}
