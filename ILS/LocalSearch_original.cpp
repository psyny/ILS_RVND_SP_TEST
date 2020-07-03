#include "LocalSearch.h" 

void LocalSearch::run(Solution & mySol)
{
	loadSolution(mySol);

	// Shuffling the order of the move evaluations at each local search
	std::shuffle(orderNodes.begin(), orderNodes.end(), params->generator);
	
	searchCompleted = false;
	bool movePerformed;
	while (!searchCompleted)
	{
		searchCompleted = true; // This flag will be set back to false every time a move is applied
		for (int posU = 0 ; posU < params->nbClients ; posU++)
		{
			nodeU = &clients[orderNodes[posU]];
			setLocalVariablesRouteU();
			movePerformed = false;
			for (int posV = 0; posV < params->nbClients && !movePerformed; posV++)
			{
				if (posU != posV)
				{
					nodeV = &clients[orderNodes[posV]];
					setLocalVariablesRouteV();
					if (!movePerformed) movePerformed = move1();						// RELOCATE
					if (!movePerformed && posU < posV) movePerformed = move4();			// SWAP
					if (!movePerformed && routeU == routeV) movePerformed = move7();	// 2-OPT
					if (!movePerformed && routeU != routeV) movePerformed = move8();	// 2-OPT*
					// PERHAPS OTHER MOVES...
					
					// DON'T FORGET SPECIAL CASES: MOVES THAT INSERT NODE-U DIRECTLY AFTER THE DEPOT
					if (!movePerformed && nodeV->prev->isDepot)
					{
						nodeV = nodeV->prev;
						setLocalVariablesRouteV();
						if (!movePerformed) movePerformed = move1();					// RELOCATE
						if (!movePerformed && routeU != routeV && !nodeV->next->isDepot) movePerformed = move8(); // 2-OPT*
						// PERHAPS OTHER MOVES...
					}

					//  DON'T FORGET SPECIAL CASES: MOVES THAT INSERT NODE-U IN AN EMPTY ROUTE (OR CUT A ROUTE IN TWO IN THE CASE OF 2-OPT* VARIANTS)
					if (!movePerformed && !emptyRoutes.empty())
					{
						nodeV = routes[*emptyRoutes.begin()].depot;
						setLocalVariablesRouteV();
						if (!movePerformed) movePerformed = move1();					// RELOCATE
						if (!movePerformed && routeU != routeV && !nodeV->next->isDepot) movePerformed = move8(); // 2-OPT*
						// PERHAPS OTHER MOVES ...
					}
				}
			}
			// Count the moves which have been applied
			if (movePerformed) searchCompleted = false;
			nbMoves += (int)movePerformed;
		}
	}

	exportSolution(mySol);
}

void LocalSearch::setLocalVariablesRouteU()
{
	routeU = nodeU->route;
	nodeX = nodeU->next;
	nodeXSuivCour = nodeX->next->cour;
	nodeUCour = nodeU->cour;
	nodeUPredCour = nodeU->prev->cour;
	nodeXCour = nodeX->cour;
	loadU    = params->clients[nodeUCour].demand;
	loadX	 = params->clients[nodeXCour].demand;
}

void LocalSearch::setLocalVariablesRouteV()
{
	routeV = nodeV->route;
	nodeY = nodeV->next;
	nodeYSuivCour = nodeY->next->cour;
	nodeVCour = nodeV->cour;
	nodeVPredCour = nodeV->prev->cour;
	nodeYCour = nodeY->cour;
	loadV    = params->clients[nodeVCour].demand;
	loadY	 = params->clients[nodeYCour].demand;
}

void LocalSearch::insertNode(Node * U, Node * V)
{
	U->prev->next = U->next;
	U->next->prev = U->prev;
	V->next->prev = U;
	U->prev = V;
	U->next = V->next;
	V->next = U;
	U->route = V->route;
}

void LocalSearch::swapNode(Node * U, Node * V)
{
	Node * myVPred = V->prev;
	Node * myVSuiv = V->next;
	Node * myUPred = U->prev;
	Node * myUSuiv = U->next;
	Route * myRouteU = U->route;
	Route * myRouteV = V->route;

	myUPred->next = V;
	myUSuiv->prev = V;
	myVPred->next = U;
	myVSuiv->prev = U;

	U->prev = myVPred;
	U->next = myVSuiv;
	V->prev = myUPred;
	V->next = myUSuiv;

	U->route = myRouteV;
	V->route = myRouteU;
}

bool LocalSearch::move1()
{
	double costSuppU = params->distanceMatrix[nodeUPredCour][nodeXCour] - params->distanceMatrix[nodeUPredCour][nodeUCour] - params->distanceMatrix[nodeUCour][nodeXCour];
	double costSuppV = params->distanceMatrix[nodeVCour][nodeUCour] + params->distanceMatrix[nodeUCour][nodeYCour] - params->distanceMatrix[nodeVCour][nodeYCour];

	if (routeU != routeV)
	{
		costSuppU += excessCharge(routeU->load - loadU) - routeU->loadPenalty;
		costSuppV += excessCharge(routeV->load + loadU) - routeV->loadPenalty;
	}

	if (costSuppU + costSuppV > -MY_EPSILON) return false;
	if (nodeUCour == nodeYCour) return false;

	insertNode(nodeU, nodeV);
	updateRouteData(routeU);
	if (routeU != routeV) updateRouteData(routeV);
	return true;
}

bool LocalSearch::move4()
{
	double costSuppU = params->distanceMatrix[nodeUPredCour][nodeVCour] + params->distanceMatrix[nodeVCour][nodeXCour] - params->distanceMatrix[nodeUPredCour][nodeUCour] - params->distanceMatrix[nodeUCour][nodeXCour];
	double costSuppV = params->distanceMatrix[nodeVPredCour][nodeUCour] + params->distanceMatrix[nodeUCour][nodeYCour] - params->distanceMatrix[nodeVPredCour][nodeVCour] - params->distanceMatrix[nodeVCour][nodeYCour];

	if (routeU != routeV)
	{
		costSuppU += excessCharge(routeU->load + loadV - loadU) 
			- routeU->loadPenalty;

		costSuppV += excessCharge(routeV->load + loadU - loadV) 
			- routeV->loadPenalty;
	}

	if (costSuppU + costSuppV > -MY_EPSILON) return false;
	if (nodeUCour == nodeVPredCour || nodeUCour == nodeYCour) return false;

	swapNode(nodeU, nodeV);
	updateRouteData(routeU);
	if (routeU != routeV) updateRouteData(routeV);
	return true;
}


bool LocalSearch::move7()
{
	if (nodeU->position > nodeV->position) return false;

	double cost = params->distanceMatrix[nodeUCour][nodeVCour] + params->distanceMatrix[nodeXCour][nodeYCour] - params->distanceMatrix[nodeUCour][nodeXCour] - params->distanceMatrix[nodeVCour][nodeYCour];

	if (cost > -MY_EPSILON) return false;
	if (nodeU->next == nodeV) return false;

	Node * nodeNum = nodeX->next;
	nodeX->prev = nodeNum;
	nodeX->next = nodeY;

	while (nodeNum != nodeV)
	{
		Node * temp = nodeNum->next;
		nodeNum->next = nodeNum->prev;
		nodeNum->prev = temp;
		nodeNum = temp;
	}

	nodeV->next = nodeV->prev;
	nodeV->prev = nodeU;
	nodeU->next = nodeV;
	nodeY->prev = nodeX;

	updateRouteData(routeU);
	return true;
}

bool LocalSearch::move8()
{
	double cost = params->distanceMatrix[nodeUCour][nodeVCour] + params->distanceMatrix[nodeXCour][nodeYCour] - params->distanceMatrix[nodeUCour][nodeXCour] - params->distanceMatrix[nodeVCour][nodeYCour]
		+ excessCharge(nodeU->cumulatedLoad + nodeV->cumulatedLoad)
		+ excessCharge(routeU->load + routeV->load - nodeU->cumulatedLoad - nodeV->cumulatedLoad)
		- routeU->loadPenalty - routeV->loadPenalty;

	if (cost > -MY_EPSILON) return false;

	Node * depotU = routeU->depot;
	Node * depotV = routeV->depot;
	Node * depotUFin = routeU->depot->prev;
	Node * depotVFin = routeV->depot->prev;
	Node * depotVSuiv = depotV->next;

	Node * temp;
	Node * xx = nodeX;
	Node * vv = nodeV;

	while (!xx->isDepot)
	{
		temp = xx->next;
		xx->next = xx->prev;
		xx->prev = temp;
		xx->route = routeV;
		xx = temp;
	}

	while (!vv->isDepot)
	{
		temp = vv->prev;
		vv->prev = vv->next;
		vv->next = temp;
		vv->route = routeU;
		vv = temp;
	}

	nodeU->next = nodeV;
	nodeV->prev = nodeU;
	nodeX->next = nodeY;
	nodeY->prev = nodeX;

	if (nodeX->isDepot)
	{
		depotUFin->next = depotU;
		depotUFin->prev = depotVSuiv;
		depotUFin->prev->next = depotUFin;
		depotV->next = nodeY;
		nodeY->prev = depotV;
	}
	else if (nodeV->isDepot)
	{
		depotV->next = depotUFin->prev;
		depotV->next->prev = depotV;
		depotV->prev = depotVFin;
		depotUFin->prev = nodeU;
		nodeU->next = depotUFin;
	}
	else
	{
		depotV->next = depotUFin->prev;
		depotV->next->prev = depotV;
		depotUFin->prev = depotVSuiv;
		depotUFin->prev->next = depotUFin;
	}

	updateRouteData(routeU);
	updateRouteData(routeV);
	return true;
}

void LocalSearch::updateRouteData(Route * myRoute)
{
	int myplace = 0;
	Node * node = myRoute->depot;
	node->position = myplace;
	double mycharge = 0.;
	node->cumulatedLoad = 0.;

	bool firstIt = true;
	while (!node->isDepot || firstIt)
	{
		firstIt = false;
		myplace++;
		node = node->next;
		node->position = myplace;
		mycharge += params->clients[node->cour].demand;
		node->cumulatedLoad = mycharge;
	}

	myRoute->load = mycharge;
	myRoute->loadPenalty = excessCharge(mycharge);
	myRoute->nbNodes = myplace;

	if (myRoute->depot->next->isDepot) emptyRoutes.insert(myRoute->cour); // If its an empty route, store it
	else emptyRoutes.erase(myRoute->cour); // Otherwise make sure its not part of the empty route vector
}

void LocalSearch::loadSolution(Solution & mySol)
{
	emptyRoutes.clear();
	nbMoves = 0;
	for (int r = 0; r < params->nbVehicles; r++)
	{
		Node * myDepot = &depots[r];
		Node * myDepotFin = &depotsEnd[r];
		Route * myRoute = &routes[r];
		myDepot->prev = myDepotFin;
		myDepotFin->next = myDepot;
		if (!mySol.sequences[r].empty())
		{
			Node * myClient = &clients[mySol.sequences[r][0]];
			myClient->route = myRoute;
			myClient->prev = myDepot;
			myDepot->next = myClient;
			for (int i = 1; i < (int)mySol.sequences[r].size(); i++)
			{
				Node * myClientPred = myClient;
				myClient = &clients[mySol.sequences[r][i]]; 
				myClient->prev = myClientPred;
				myClientPred->next = myClient;
				myClient->route = myRoute;
			}
			myClient->next = myDepotFin;
			myDepotFin->prev = myClient;
		}
		else
		{
			myDepot->next = myDepotFin;
			myDepotFin->prev = myDepot;
		}
		updateRouteData(&routes[r]);
	}
}

void LocalSearch::exportSolution(Solution & mySol)
{
	int pos = 0;
	for (int r = 0; r < params->nbVehicles; r++)
	{
		mySol.sequences[r].clear();
		Node * node = depots[r].next;
		while (!node->isDepot)
		{
			mySol.sequences[r].push_back(node->cour);
			node = node->next;
			pos++;
		}
	}
	mySol.evaluateCost();
}

LocalSearch::LocalSearch(Params * params) : params (params)
{
	clients = std::vector < Node >(params->nbClients + 1);
	routes = std::vector < Route >(params->nbVehicles);
	depots = std::vector < Node >(params->nbVehicles);
	depotsEnd = std::vector < Node >(params->nbVehicles);
	for (int i = 0; i <= params->nbClients; i++) 
	{ 
		clients[i].cour = i; 
		clients[i].isDepot = false; 
	}
	for (int i = 0; i < params->nbVehicles; i++)
	{
		routes[i].cour = i;
		routes[i].depot = &depots[i];
		depots[i].cour = 0;
		depots[i].isDepot = true;
		depots[i].route = &routes[i];
		depotsEnd[i].cour = 0;
		depotsEnd[i].isDepot = true;
		depotsEnd[i].route = &routes[i];
	}
	for (int i = 1 ; i <= params->nbClients ; i++) 
		orderNodes.push_back(i);
}

