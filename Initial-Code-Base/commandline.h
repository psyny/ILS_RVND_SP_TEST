#ifndef COMMAND_LINE_H
#define COMMAND_LINE_H

#include <iostream>
#include <string>

class commandline
{

private:

	bool command_ok;				// Tells whether the line of command is valid
	int cpu_time;					// Allocated CPU time
	int seed;						// Random seed
	std::string instance_name;		// Instance path
	std::string output_name;		// Output path

public:

	// Constructor
	commandline(int argc, char* argv[])
	{
		command_ok = true;
		if (argc % 2 != 0 || argc > 8 || argc < 2)
		{
			std::cout << "----- NUMBER OF COMMANDLINE PARAMETERS IS TOO SMALL OR TOO BIG: " << argc << std::endl;
			command_ok = false;
		}
		else if (std::string(argv[1]) == "-?" || std::string(argv[1]) == "-help")
		{
			command_ok = false;
		}
		else
		{
			// Setting some default values
			instance_name = std::string(argv[1]);
			output_name = "solution.sol";
			cpu_time = 360;
			seed = 0;

			for (int i = 2; i < argc; i += 2)
			{
				if (std::string(argv[i]) == "-t")
					cpu_time = atoi(argv[i + 1]);
				else if (std::string(argv[i]) == "-sol")
					output_name = std::string(argv[i + 1]);
				else if (std::string(argv[i]) == "-seed")
					seed = atoi(argv[i + 1]);
				else
				{
					std::cout << "----- NON RECOGNIZED ARGUMENT: " << std::string(argv[i]) << std::endl;
					command_ok = false;
				}
			}
		}
		if (!command_ok)
			display_help();
	}

	// Printing information message about correct commandline
	void display_help()
	{
		std::cout << std::endl;
		std::cout << "-------------------------------------------- COMMANDLINE -------------------------------------------------------" << std::endl;
		std::cout << "Usage: ./genvrp instancePath [-sol solPath] [-type instanceType] [-t myCPUtime]                                 " << std::endl;
		std::cout << "[-sol solPath] sets the output solution path. Defaults to the instance file name prepended with sol-            " << std::endl;
		std::cout << "[-t myCPUtime] sets the CPU time limit in seconds. Defaults to t=360 seconds                                    " << std::endl;
		std::cout << "[-seed mySeed] sets a fixed seed. Defaults to 0                                                                 " << std::endl;
		std::cout << "----------------------------------------------------------------------------------------------------------------" << std::endl;
		std::cout << std::endl;
	};

	// Getting path to the instance file
	std::string get_path_to_instance() { return instance_name; }

	// Getting path to the solution file
	std::string get_path_to_solution() { return output_name; }

	// Getting allocated CPU time
	int get_cpu_time() { return cpu_time; }

	// Getting the random seed
	int get_seed() { return seed; }

	// Tests whether the commandline parameters are OK
	bool is_valid() { return command_ok; }
};
#endif
