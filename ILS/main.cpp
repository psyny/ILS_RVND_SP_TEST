#include "Solver.h"
#include "commandline.h"
#include "ILSRVND.h"
using namespace std;

bool runInstance(commandline& c, Params& params, std::string inputFile, std::string outputFile) {
	std::cout << "----- INSTANCE LOADED: " << inputFile << " WITH " << params.nbClients << " CUSTOMERS AND " << params.nbVehicles << " VEHICLES -----" << std::endl;

	std::cout << "----- START OF THE ALGORITHM -----" << std::endl;
	params.startTime = clock();
	params.timeLimit = c.get_cpu_time() * CLOCKS_PER_SEC;
	Solution mySolution(&params);				// Creating a solution object
	Solver mySolver(&params);					// Creating a solver object
	mySolver.run(mySolution);
	params.endTime = clock();
	std::cout << "----- END OF THE ALGORITHM -----" << std::endl;

	// Exporting the best solution
	if(outputFile != "") mySolution.exportToFile(outputFile);

	return true;
}


int main(int argc, char *argv[])
{
	commandline c(argc, argv);
	if (c.is_valid())
	{
		if (c.is_runall()) {
			// Run all listed instances
			std::list<std::string> files({ 
				"A-n32-k5", "A-n63-k10","X-n101-k25","X-n106-k14","X-n110-k13","X-n115-k10",
				"X-n120-k6","X-n125-k30","X-n129-k18","X-n134-k13","X-n139-k10","X-n143-k7",
				"X-n148-k46","X-n153-k22","X-n157-k13","X-n162-k11","X-n167-k10","X-n172-k51",
				"X-n176-k26","X-n181-k23","X-n186-k15","X-n190-k8","X-n195-k51","X-n200-k36",
				"X-n204-k19","X-n209-k16","X-n214-k11","X-n219-k73","X-n223-k34","X-n228-k23",
				"X-n233-k16","X-n237-k14","X-n242-k48","X-n247-k50","X-n251-k28","X-n256-k16",
				"X-n261-k13","X-n266-k58","X-n270-k35","X-n275-k28","X-n280-k17","X-n284-k15",
				"X-n289-k60","X-n294-k50","X-n298-k31","X-n303-k21","X-n308-k13","X-n313-k71",
				"X-n317-k53","X-n322-k28","X-n327-k20","X-n331-k15","X-n336-k84","X-n344-k43",
				"X-n351-k40","X-n359-k29","X-n367-k17","X-n376-k94","X-n384-k52","X-n393-k38",
				"X-n401-k29","X-n411-k19","X-n420-k130","X-n429-k61","X-n439-k37","X-n449-k29",
				"X-n459-k26","X-n469-k138","X-n480-k70","X-n491-k59","X-n502-k39","X-n513-k21",
				"X-n524-k153","X-n536-k96","X-n548-k50","X-n561-k42","X-n573-k30","X-n586-k159",
				"X-n599-k92","X-n613-k62","X-n627-k43","X-n641-k35","X-n655-k131","X-n670-k130",
				"X-n685-k75","X-n701-k44","X-n716-k35","X-n733-k159","X-n749-k98","X-n766-k71",
				"X-n783-k48","X-n801-k40","X-n819-k171","X-n837-k142","X-n856-k95","X-n876-k59",
				"X-n895-k37","X-n916-k207","X-n936-k151","X-n957-k87","X-n979-k58","X-n1001-k43"
				});

			// Run all instances
			std::string instancesFolder = "../Instances/";
			int rolls = 0;
			for (std::string fileName : files) {
				std::string instanceFile = instancesFolder + fileName + ".vrp";
				std::string outputFile = fileName + ".sol";

				Params params(instanceFile, c.get_seed());
				runInstance(c, params, instanceFile, outputFile);
			}
		}
		else if (c.is_test()) {
			// Test Process 01
			Params params(c.get_path_to_instance(), c.get_seed());
			std::cout << "----- INSTANCE LOADED: " << c.get_path_to_instance() << " WITH " << params.nbClients << " CUSTOMERS AND " << params.nbVehicles << " VEHICLES -----" << std::endl;

			std::cout << "----- TEST PROCEDURES -----" << std::endl;
			Solver testSolver(&params);
			testSolver.runTests(c.get_path_to_solution());
		} 
		else
		{
			// Run only given solution
			Params params(c.get_path_to_instance(), c.get_seed());
			runInstance(c, params, c.get_path_to_instance(), c.get_path_to_solution());
		}
	}
	return 0;
}

