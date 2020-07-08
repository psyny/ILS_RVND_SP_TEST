#ifndef NEIGHBSTATUS_H
#define NEIGHBSTATUS_H

#include "Solution.h"

class NeighbStatus
{
public:
	bool hadInitialOptimization;
	std::vector<std::vector<bool>> status;

	// Constructor
	NeighbStatus(Solution& mySol);

	// Route was related to an improvement. The route itself can have its cost raise, but the total solution improved.
	// Result: All moves of the route is set to true 
	void routeRelatedToImprove(int routeUid);

	// Set state of a move of a route
	void setMoveRouteState(int moveId, int routeId,bool newState = false);

	// Check if worth doing a move between two routes
	bool worthMoveBetween(int moveId, int routeUid, int routeVid);
};

#endif
