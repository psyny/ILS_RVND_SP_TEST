#include "Solution.h" 

// Generate an initial solution for the tests
void Solution::initializeTestSolution()
{
	// Number of clients per route
	int routeA_clients = 6;
	int routeB_clients = 4;
	int routeC_clients = 3;

	if (routeA_clients > params->nbClients) routeA_clients = params->nbClients;
	if (routeB_clients > params->nbClients) routeB_clients = params->nbClients;
	if (routeC_clients > params->nbClients) routeC_clients = params->nbClients;
	
	// Fixed parameters
	params->nbVehicles = 3; // Force to use only 3 vehicles
	sequences = std::vector < std::vector <int> >(params->nbVehicles);

	// Build Route A
	int firstClient = 1;
	int lastClient = routeA_clients;
	for (int i = firstClient; i <= lastClient; i++) {
		sequences[0].push_back(i);
	}

	// Build Route B
	firstClient = lastClient + 1;
	lastClient = firstClient + routeB_clients - 1;
	for (int i = firstClient; i <= lastClient; i++) {
		sequences[1].push_back(i);
	}

	// Build Route C
	firstClient = lastClient + 1;
	lastClient = firstClient + routeC_clients - 1;
	for (int i = firstClient; i <= lastClient; i++) {
		sequences[2].push_back(i);
	}
}

// Initial Valid Solution
void Solution::initializeSweep()
{
	sequences = std::vector < std::vector <int> >(params->nbVehicles);

	// Order the customer visits by angle around the depot
	std::vector <std::pair <double, int>> polarAngles;
	for (int i = 1; i <= params->nbClients; i++)
		polarAngles.push_back(std::pair <double, int>(atan2(params->clients[i].coordY - params->clients[0].coordY, params->clients[i].coordX - params->clients[0].coordX), i));
	std::sort(polarAngles.begin(), polarAngles.end());

	// Add the customers into the current route in this order, as long as the capacity limit is satisfied
	int routeIndex = 0;
	double routeLoad = 0.;
	std::uniform_int_distribution<> myDistribution(0, params->nbClients-1); // Defining the distribution
	int shift = myDistribution(params->generator); // Sampling a number within this distribution using our RNG
	for (int i = 0; i < params->nbClients;)
	{
		int myCustomer = polarAngles[(i + shift) % params->nbClients].second;
		if (routeLoad + params->clients[myCustomer].demand < params->vehicleCapacity + MY_EPSILON || routeIndex == params->nbVehicles - 1)
		{
			sequences[routeIndex].push_back(myCustomer);
			routeLoad += params->clients[myCustomer].demand;
			i++;
		}
		else
		{
			routeIndex++;
			routeLoad = 0.;
		}
	}
	// Evaluate the resulting solution cost
	evaluateCost();
}

void Solution::evaluateCost()
{
	totalDistance = 0.;
	nbRoutes = 0;
	double totalCapacityViolation = 0.;

	for (int r = 0; r < params->nbVehicles; r++)
	{
		if (!sequences[r].empty())
		{
			double routeDistance = params->distanceMatrix[0][sequences[r][0]];
			double routeLoad = params->clients[sequences[r][0]].demand;
			for (int i = 1; i < (int)sequences[r].size(); i++)
			{
				routeDistance += params->distanceMatrix[sequences[r][i - 1]][sequences[r][i]];
				routeLoad += params->clients[sequences[r][i]].demand;
			}
			routeDistance += params->distanceMatrix[sequences[r][sequences[r].size() - 1]][0];

			totalDistance += routeDistance;
			nbRoutes++;
			if (routeLoad > params->vehicleCapacity)
				totalCapacityViolation += routeLoad - params->vehicleCapacity;
		}
	}

	penalizedCost = totalDistance;
	isFeasible = (totalCapacityViolation < MY_EPSILON);
}

void Solution::exportToFile(std::string pathToSolution)
{
	std::ofstream myfile(pathToSolution);
	if (myfile.is_open())
	{
		if (isFeasible)
		{
			std::cout << "----- WRITING SOLUTION WITH VALUE " << penalizedCost << " TO : " << pathToSolution << std::endl;
			
			myfile << "COST: " << penalizedCost << std::endl;                                           // Exporting solution value
			myfile << "NB_ROUTES: " << nbRoutes << std::endl;                                           // Exporting the number of routes
			myfile << "TIME(s): " << (double)(params->endTime-params->startTime) / (double)CLOCKS_PER_SEC << std::endl;       // Exporting the total CPU time in seconds

			// Printing the routes
			int routeIndex = 1;
			for (int k = 0; k < params->nbVehicles; k++)
			{
				if (!sequences[k].empty())
				{
					myfile << "ROUTE " << routeIndex << " WITH " << sequences[k].size() << " CUSTOMER VISITS: 0";
					for (int i = 0; i < sequences[k].size(); i++)
						myfile << " " << sequences[k][i];
					myfile << " 0" << std::endl;
					routeIndex++;
				}
			}
		}
		else
		{
			std::cout << "----- FINAL SOLUTION IS INFEASIBLE " << std::endl;
			myfile << "[INFEASIBLE]" << std::endl;
		}
		myfile.close();
	}
	else
	{
		std::cout << "----- IMPOSSIBLE TO OPEN SOLUTION FILE: " << pathToSolution << " ----- " << std::endl;
	}
}
