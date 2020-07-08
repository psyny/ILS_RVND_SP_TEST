#include "NeighbStatus.h"
#include "RVND.h"

// -----------------------------------------------------------------------------------

// Constructor
NeighbStatus::NeighbStatus(Solution& mySol)
{
	hadInitialOptimization = false;
	status = std::vector<std::vector<bool>>(mySol.sequences.size());

	for (int i = 0; i < status.size(); i++)
	{
		status[i] = std::vector<bool>(INTERROUTE_MOVES,true);
	}
}

// Route was related to an improvement. The route itself can have its cost raise, but the total solution improved.
// Result: All moves of the route is set to true 
void NeighbStatus::routeRelatedToImprove(int routeUid)
{
	std::fill(status[routeUid].begin(), status[routeUid].end(), true);
}


// Set state of a move of a route
void NeighbStatus::setMoveRouteState(int moveId, int routeId, bool newState)
{
	status[routeId][moveId] = newState;
}

// Check if worth doing a move between two routes
bool NeighbStatus::worthMoveBetween(int moveId, int routeUid, int routeVid) {
	bool isWorth = status[routeUid][moveId] == true || status[routeVid][moveId] == true;
	return isWorth;
}

