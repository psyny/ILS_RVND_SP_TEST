#ifndef PARAMS_H
#define PARAMS_H

#include <string>
#include <vector>
#include <list>
#include <unordered_set>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <math.h>
#include <time.h>
#include <algorithm>
#include <random>
#define MY_EPSILON 0.00001 // Precision parameter, useful to avoid numerical instabilities

struct Client
{
	int custNum;			// Customer Index
	double coordX;			// Coordinate X
	double coordY;			// Coordinate Y
	double demand;			// Demand
};

class Params
{
public:

	/* GENERAL PARAMETERS */
	clock_t startTime ;				// Time when the algorithm started
	clock_t endTime;				// Time when the algorithm finished
	clock_t timeLimit;              // Duration allowed for the algorithm

	/* RANDOM NUMBER GENERATOR */
	std::default_random_engine generator;				// Random number generator
	int seed;											// Seed used to initialize the generator

	/* DATA OF THE CVRP INSTANCE */
	int nbClients ;												// Number of customers
	int nbVehicles ;											// Number of vehicles
	double totalDemand ;										// Total demand required by the clients
	double vehicleCapacity;										// Capacity limit
	std::vector < Client > clients ;							// Vector containing information on each customer -- clients[0] is the depot -- clients[i] for i in 1...nbClients are the customers 
	std::vector < std::vector < double > > distanceMatrix ;		// Distance matrix

	/* TESTING */
	bool testRoutine;

	/* PARSING ROUTINES */

	/* METHOD HYPER PARAMETERS */
	int maxiter_b;
	int maxiterILS_b;
	int maxiterRVND_b;

	/* METHOD DISCOVERED PARAMETERS */
	int numberOfVehicles;
	double tolerance;

	// Setting the parameters of the method
	void setMethodParams();

	// Constructor
	Params(std::string pathToInstance, int seedRNG);
};
#endif

