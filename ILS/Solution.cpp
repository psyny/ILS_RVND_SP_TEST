#include "Solution.h" 
#include "InsertionCriterion.h"
#include "InsertionCriterionMCFIC.h"
#include "InsertionCriterionNIFC.h"
#include "InsertionStrategy.h"
#include "InsertionStrategySIS.h"
#include "InsertionStrategyPIS.h"

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

	// Evaluate the resulting solution cost
	evaluateCost();
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

void Solution::initializeSolution() {
	if (MAX_SOLUTION_ATTEMPTS == 0) {
		isFeasible = false;
		return;
	}

	int attempts = 0;
	do {
		attempts++;

		// Initialize the CL
		std::vector<int> cl; // ToDo maybe change it to a set or a linkedlist?

		// Starts from 1, ignoring the depot.
		for (int i = 1; i < params->clients.size(); i++) {
			cl.push_back(params->clients[i].custNum);
		}

		// Shuffle the CL list
		std::shuffle(cl.begin(), cl.end(), params->generator);

		// Each route is filled with a seed customer k, randomly selected from the CL
		sequences = std::vector < std::vector <int> >(params->nbVehicles);
		for (int i = 0; i < sequences.size(); i++) {
			addClient(i, cl.back());
			cl.pop_back();               // Removes the last element
		}

		// InsertionCriterion
		std::uniform_int_distribution<int> dist_crit(0,1);
		int criterion = dist_crit(params->generator);
		InsertionCriterion* insertionCriterion;
		if (criterion == 0) insertionCriterion = (InsertionCriterion*) new InsertionCriterionMCFIC();
		else insertionCriterion = (InsertionCriterion*) new InsertionCriterionNIFC();

		// InsertionStrategy
		std::uniform_int_distribution<int> dist_strat(0, 1);
		int strategy = dist_strat(params->generator);
		InsertionStrategy* insertionStrategy;
		if (criterion == 0) insertionStrategy = (InsertionStrategy*) new InsertionStrategySIS();
		else insertionStrategy = (InsertionStrategy*) new InsertionStrategyPIS();

		// Execute the strategy
		insertionCriterion = (InsertionCriterion*) new InsertionCriterionMCFIC();
		insertionStrategy = (InsertionStrategy*) new InsertionStrategySIS();

		insertionStrategy->execute(params, this, &cl, insertionCriterion);

		evaluateCost();
	} while (!isFeasible && attempts < MAX_SOLUTION_ATTEMPTS);

	return;
}

void Solution::evaluateCost()
{
	isFeasible = true;
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
				isFeasible = false;
		}
	}

	penalizedCost = totalDistance;
	//isFeasible = false;
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

void Solution::addClient(int routeId, int clientId) {
	sequences[routeId].push_back(clientId);
}

void Solution::addClient(int routeId, int clientId, int pos) {
	sequences[routeId].insert(sequences[routeId].begin() + pos, clientId);
}

std::vector<int>* Solution::getRoute(int routeId) {
	return &sequences[routeId];
}
