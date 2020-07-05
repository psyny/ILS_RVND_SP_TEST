#ifndef SOLUTION_H
#define SOLUTION_H

#include "Params.h"

class Solution
{

private:

  // Access to the problem parameters
  Params * params;											// Problem parameters

public:

  // Solution characteristics
  bool   isFeasible;						 				// Feasibility status of the solution
  int    nbRoutes;			                 				// Number of non-empty routes
  double totalDistance;										// Total distance
  double penalizedCost;										// Solution cost (including possible penalties for infeasibility -- only for some solution methods)
  std::vector < std::vector <int> > sequences ;				// For each vehicle, the associated sequence of customer deliveries (can be possibly empty in case of empty routes)
  
  // Evaluates the cost of the solution and calculates the other values
  void evaluateCost();

  // Exports this solution to a file
  void exportToFile(std::string pathToSolution);

  // Builds an initial solution with the SWEEP algorithm
  void initializeSweep();

  // Builds an initial solution to test ILS moves 
  void initializeTestSolution();

  // Builds an initial solution according to the one proposed on the ILS-RVND paper
  void initializeSolution();

  // Adds a client at of the route
  void addClient(int routeId, int clientId);

  // Adds a client at a specific position
  void addClient(int routeId, int clientId, int pos);

  // Get a route sequence
  std::vector<int>* getRoute(int routeId);


  // Constructor: Empty solution as a container
  Solution(Params * params): params(params)
  {
	  penalizedCost = 1.e30;
	  isFeasible = false;
  }
};
#endif
