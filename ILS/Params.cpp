#include "Params.h"

Params::Params(std::string pathToInstance, int seedRNG): seed(seedRNG)
{
	// Initializing the random seed
	generator.seed(seedRNG);

	// Setting some parameters
	setMethodParams();

	// Reading the instance
	std::ifstream inputFile(pathToInstance.c_str());
	if (inputFile.is_open())
	{
		std::string content;
		int useless;
		totalDemand = 0.;
		getline(inputFile, content);
		getline(inputFile, content);
		getline(inputFile, content);
		inputFile >> content;
		inputFile >> content;
		inputFile >> nbClients;
		nbClients--; // The file indicates the total number of nodes (including the depot)
		getline(inputFile, content);
		getline(inputFile, content);
		inputFile >> content;
		inputFile >> content;
		inputFile >> vehicleCapacity;
		getline(inputFile, content);
		getline(inputFile, content);

		// Read customer information
		clients = std::vector<Client>(nbClients + 1);
		for (int i = 0; i <= nbClients; i++)
		{
			inputFile >> clients[i].custNum >> clients[i].coordX >> clients[i].coordY;
			clients[i].custNum--; // Depot index should be 0, and (1...n) are customers
		}

		// Customer demands are listed at the end of the file
		getline(inputFile, content);
		getline(inputFile, content);
		for (int i = 0; i <= nbClients; i++)
		{
			inputFile >> useless;
			inputFile >> clients[i].demand;
			totalDemand += clients[i].demand;
		}

		// Evaluate the minimum number of vehicles necessary to cover the demand and define a fleet of reasonable size
		// Default value: we give 20%+2 more vehicles than the minimum possible, this is largely enough for all instances
		int lbVehicles = std::ceil(totalDemand / vehicleCapacity);
		nbVehicles = std::ceil((double)lbVehicles*1.2) + 2;
	}
	else
		throw std::invalid_argument("Cannot open instance file: " + pathToInstance);

	// Computing the distance matrix
	distanceMatrix = std::vector < std::vector< double > >(nbClients + 1, std::vector <double>(nbClients + 1));
	for (int i = 0; i <= nbClients; i++)
	{
		for (int j = 0; j <= nbClients; j++)
		{
			double d = std::sqrt((clients[i].coordX - clients[j].coordX)*(clients[i].coordX - clients[j].coordX) + (clients[i].coordY - clients[j].coordY)*(clients[i].coordY - clients[j].coordY));
			d += 0.5; d = (double)(int)d; // integer rounding should be used for the X instances
			distanceMatrix[i][j] = d;
		}
	}

	// Test Routine
	testRoutine = false;
}

void Params::setMethodParams()
{
	// You can define some method parameters here if you wish
	maxiter_b = 100;
	maxiterILS_b = 2000; // How many pertubations ILS will do
	maxStartsILS_b = 1; 
}
