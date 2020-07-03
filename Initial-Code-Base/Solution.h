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

  // Constructor: Empty solution as a container
  Solution(Params * params): params(params)
  {
	  penalizedCost = 1.e30;
	  isFeasible = false;
  }
};
#endif
