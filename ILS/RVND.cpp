#include "RVND.h" 
// ----------------------------------------------------------------------------------
// This first segment is here to test route operations

// Function to check if the moves are working as intended
void RVND::runTests(Solution& mySol, std::string exportFile)
{
	loadSolution(mySol);
	std::vector < std::string > results = std::vector < std::string >();
	params->testRoutine = true;

	// Save the state of the given solution
	routesToString("Initial State: ", results);

	MoveCheck moveCheck;
	int nodeUid;
	int nodeVid;

	// ---------------------------- Inter-Route Moves
	results.push_back("Inter-Route Moves ------------------------------------------------");
	results.push_back("");

	// Shift (1,0)
	nodeUid = 1;
	nodeVid = 7;
	prepareNodes(nodeUid, nodeVid);
	moveCheck = shift10_check();
	if(moveCheck.isValid == true ) shift10_do();
	routesToString("Shift(1,0), " + std::to_string(nodeUid) + " to " + std::to_string(nodeVid) + ": ", results);

	/*
	// Shift (2,0)
	prepareNodes(7, 2);
	shift20();
	routesToString("Shift(2,0), 7 to 2: ", results);

	// Shift (3,0)
	prepareNodes(2, 9);
	shift30();
	routesToString("Shift(3,0), 2 to 9: ", results);

	// Swap (1,1)
	prepareNodes(5, 9);
	swap11();
	routesToString("Swap(1,1): 5 to 9 ", results);

	// Swap (2,1)
	prepareNodes(4, 7);
	swap21();
	routesToString("Swap(2,1): 4 to 7 ", results);

	// Swap (2,2)
	prepareNodes(4, 7);
	swap22();
	routesToString("Swap(2,2): 4 to 7 ", results);

	// ---------------------------- Intra-Route Moves
	results.push_back("Intra-Route Moves ------------------------------------------------");
	results.push_back("");

	// Reinsertion
	prepareNodes(8, 10);
	reinsertion();
	routesToString("Reinsertion, 8 to 10: ", results);

	// Or-Opt2
	prepareNodes(1, 2);
	oropt2();
	routesToString("Or-Opt2, 1 to 2: ", results);

	// Or-Opt3
	prepareNodes(5, 7);
	oropt3();
	routesToString("Or-Opt3, 5 to 7: ", results);
	*/

	// ---------------------------- Sweeps
	MoveInfo bestMoveInfo;
	bestMoveInfo = shift10_sweep();
	results.push_back("Best Move Info: ");
	results.push_back("CostChange: " + std::to_string(bestMoveInfo.costChange) );
	results.push_back("NodeUid: " + std::to_string(bestMoveInfo.nodeUcour) );
	results.push_back("NodeVid: " + std::to_string(bestMoveInfo.nodeVcour) );
	

	// ---------------------------- Export to external file
	exportTestToFile(results, exportFile);
}

void RVND::prepareNodes(int nudeIdU, int nudeIdV)
{
	nodeU = &clients[nudeIdU];
	setLocalVariablesRouteU();

	nodeV = &clients[nudeIdV];
	setLocalVariablesRouteV();
}

void RVND::routesToString(std::string header, std::vector < std::string > & results)
{
	results.push_back(header);
	double totalCost = 0;
	std::string costString = "";

	for (int r = 0; r < params->nbVehicles; r++)
	{
		// Clients visited
		std::string routeString = "R" + std::to_string(r+1) + ": 0";

		Node* node = depots[r].next;
		double routeCost = params->distanceMatrix[node->prev->cour][node->cour];
		while (!node->isDepot)
		{
			routeString += " " + std::to_string(node->cour);
			routeCost += params->distanceMatrix[node->cour][node->next->cour];
			node = node->next;
		}
		routeString += " 0 ";

		// Attach more info ( Cost and Load )
		routeString += "\t[";

		costString = std::to_string(routeCost);
		costString = costString.substr(0, costString.find(".") + 3);
		routeString += "cost: " + costString;
		
		costString = std::to_string(node->route->load);
		costString = costString.substr(0, costString.find(".") + 0);
		routeString += "  \tload: " + costString;

		routeString += "]";

		// Route cost
		results.push_back(routeString);

		// Update total cost
		totalCost += routeCost;
	}

	// Register Total Cost
	costString = std::to_string(totalCost);
	costString = costString.substr(0, costString.find(".") + 3);
	results.push_back("Total Cost: " + costString);

	results.push_back("");
}

void RVND::exportTestToFile(std::vector < std::string > & results, std::string exportFile)
{
	std::ofstream myfile(exportFile);
	if (myfile.is_open())
	{
		for (int i = 0; i < results.size(); i++)
		{
			myfile << "" << results[i] << std::endl;
		}

		myfile.close();
	}
	else
	{
		std::cout << "----- IMPOSSIBLE TO OPEN TEST RESULTS FILE: " << exportFile << " ----- " << std::endl;
	}
}

// -----------------------------------------------------------------------------------

// Algorithm run
void RVND::run(Solution & mySol)
{
	loadSolution(mySol); 

	// UPDATE ADS
	// WHY?

	// Init inter route NL
	populateInterRouteNL();

	// Main Loop
	while (interRouteNL.size() > 0)
	{
		// Choose a random move to create a neighborhoood (the list init is random)
		int moveId = interRouteNL.back();

		// Find the best solution parameters on the neighborhood
		moveId = 0; // DEBUG: only while we dont implement all features
		MoveInfo moveInfo = getBestInterRouteMove(moveId);

		// Check if the move is good
		if (moveInfo.costChange <= -MY_EPSILON)
		{
			doInterMoveOnCurrentSolution(moveId, moveInfo);
		}
		else
		{
			// Remove move from NL
			interRouteNL.pop_back();
		}



	}

	// ----------------------------------


	for (int iterILS = 0; iterILS < params->maxiterILS_b; iterILS++)
	{
		// PREP 1: Generate Initial Solution
		Solution startingSolution(params);
		startingSolution.initializeSweep(); // TODO: Replace for a better way

		// STEP 2: RVND Loop
		for (int iterRVND = 0; iterRVND < params->maxiterRVND_b; iterRVND++)
		{
			// STEP 2.1: Get solution from RVND
			// TODO

			// STEP 2.2: Solution Update
			// TODO

			// STEP 2.3: Perturbation 
			// TODO
		}

		// STEP 3: Solution check and update
		// TODO
	}

	exportSolution(mySol);
}

void RVND::setLocalVariablesRouteU()
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

void RVND::setLocalVariablesRouteV()
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

void RVND::insertNode(Node * U, Node * V)
{
	U->prev->next = U->next;
	U->next->prev = U->prev;
	V->next->prev = U;
	U->prev = V;
	U->next = V->next;
	V->next = U;

	U->route = V->route;
}

void RVND::insertNode2(Node* U, Node* V)
{
	Node* X = U->next;

	U->prev->next = X->next;
	X->next->prev = U->prev;
	V->next->prev = X;
	U->prev = V;
	X->next = V->next;
	V->next = U;

	U->route = V->route;
	X->route = V->route;
}

void RVND::insertNode3(Node* U, Node* V)
{
	Node* X = U->next;
	Node* W = X->next;

	U->prev->next = W->next;
	W->next->prev = U->prev;
	V->next->prev = W;
	U->prev = V;
	W->next = V->next;
	V->next = U;

	U->route = V->route;
	X->route = V->route;
	W->route = V->route;
}

void RVND::swapNode(Node * U, Node * V)
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

void RVND::swapNode2(Node* U, Node* V)
{
	// Other related nodes
	Node* X = U->next;

	// Saving infos
	Node* Vprev = V->prev;
	Node* Vnext = V->next;
	Route* RU = U->route;
	Route* RV = V->route;

	// Switching references
	V->prev = U->prev;
	U->prev->next = V;
	V->next = X->next;
	X->next->prev = V;

	U->prev = Vprev;
	Vprev->next = U;
	X->next = Vnext;
	Vnext->prev = X;

	// Updading routes
	U->route = RV;
	X->route = RV;
	V->route = RU;
}

void RVND::swapNode22(Node* U, Node* V)
{
	// Other related nodes
	Node* X = U->next;
	Node* Y = V->next;

	// Saving infos
	Node* Vprev = V->prev;
	Node* Ynext = Y->next;
	Route* RU = U->route;
	Route* RV = V->route;

	// Switching references
	V->prev = U->prev;
	U->prev->next = V;
	Y->next = X->next;
	X->next->prev = Y;

	U->prev = Vprev;
	Vprev->next = U;
	X->next = Ynext;
	Ynext->prev = X;

	// Updading routes
	U->route = RV;
	X->route = RV;
	V->route = RU;
	Y->route = RU;
}

// --------------------------------------------------- MOVES
// Insert given Node U and its next Node X after the given Node V
MoveCheck RVND::shift10_check()
{
	MoveCheck moveCheck{ false, 0 };

	// Check if the move is valid
	if (nodeUCour == nodeYCour) return moveCheck;

	// Inner load check heuristic
	double routeUloadTransfer = params->clients[nodeUCour].demand;
	if (routeUloadTransfer + routeV->load > params->vehicleCapacity) return moveCheck;

	// Costs calculations
		// Route U
		double costUPredX = params->distanceMatrix[nodeUPredCour][nodeXCour];
		double costUPredU = params->distanceMatrix[nodeUPredCour][nodeUCour];
		double costUX = params->distanceMatrix[nodeUCour][nodeXCour];

		double routeUGain = costUPredX;
		double routeULoss = costUPredU + costUX;
		double routeUBalance = routeUGain - routeULoss;

		// Route V
		double costVY = params->distanceMatrix[nodeVCour][nodeYCour];
		double costVU = params->distanceMatrix[nodeVCour][nodeUCour];
		double costUY = params->distanceMatrix[nodeUCour][nodeYCour];

		double routeVGain = costVU + costUY;
		double routeVLoss = costVY;
		double routeVBalance = routeVGain - routeVLoss;
		
	// Return Struct
	moveCheck.isValid = true;
	moveCheck.costChange = routeUBalance + routeVBalance;
	return moveCheck;
}

bool RVND::shift10_do()
{
	insertNode(nodeU, nodeV);
	updateRouteData(routeU);
	if (routeU != routeV) updateRouteData(routeV);
	return true;
}

// Insert given Node U and its next Node X after the given Node V
bool RVND::shift20()
{
	// Check if the move is valid
	if (nodeUCour == nodeYCour) return false;
	if (nodeU->next->isDepot) return false;

	// Inner load check heuristic
	double routeUloadTransfer = params->clients[nodeUCour].demand + params->clients[nodeXCour].demand;
	if (routeUloadTransfer + routeV->load > params->vehicleCapacity) return false;

	// Costs calculations
		// Route U
		int nodePostXCour = nodeX->next->cour;
		double costPredUU = params->distanceMatrix[nodeUPredCour][nodeUCour];
		double costUX = params->distanceMatrix[nodeUCour][nodeXCour];
		double costXPostX = params->distanceMatrix[nodeXCour][nodePostXCour];
		double costPredUPostX = params->distanceMatrix[nodeUPredCour][nodePostXCour];

		double routeUGain = costPredUPostX;
		double routeULoss = costPredUU + costUX + costXPostX;
		double routeUBalance = routeUGain - routeULoss;

		// Route V
		double costVY = params->distanceMatrix[nodeVCour][nodeYCour];
		double costVU = params->distanceMatrix[nodeVCour][nodeUCour];
		double costXY = params->distanceMatrix[nodeXCour][nodeYCour];

		double routeVGain = costVU + costUX + costXY;
		double routeVLoss = costVY;
		double routeVBalance = routeVGain - routeVLoss;

	// Check if the move is worth
	if (routeVBalance + routeUBalance > -MY_EPSILON && params->testRoutine == false) return false;
	// TODO: Check LOAD

	// Make the move
	insertNode2(nodeU, nodeV);
	updateRouteData(routeU);
	if (routeU != routeV) updateRouteData(routeV);
	return true;
}

// Insert given Node U and its two next Nodes X and W after the given Node V
bool RVND::shift30()
{
	// Check if the move is valid
	if (nodeUCour == nodeYCour) return false;
	if (nodeU->next->isDepot) return false;
	if (nodeU->next->next->isDepot) return false;

	// Inner load check heuristic
	int nodeWCour = nodeX->next->cour; // Node W = Node X -> Next ( the last node on string to be transfered )
	double routeUloadTransfer = params->clients[nodeUCour].demand
		+ params->clients[nodeXCour].demand
		+ params->clients[nodeWCour].demand;
	if (routeUloadTransfer + routeV->load > params->vehicleCapacity) return false;

	// Costs calculations
		// Route U
		int nodePostWCour = nodeX->next->next->cour;
		double costPredUU = params->distanceMatrix[nodeUPredCour][nodeUCour];
		double costUX = params->distanceMatrix[nodeUCour][nodeXCour];
		double costXW = params->distanceMatrix[nodeXCour][nodeWCour];
		double costWPostW = params->distanceMatrix[nodeWCour][nodePostWCour];
		double costPredUPostW = params->distanceMatrix[nodeUPredCour][nodePostWCour];

		double routeUGain = costPredUPostW;
		double routeULoss = costPredUU + costUX + costXW + costWPostW;
		double routeUBalance = routeUGain - routeULoss;

		// Route V
		double costVY = params->distanceMatrix[nodeVCour][nodeYCour];
		double costVU = params->distanceMatrix[nodeVCour][nodeUCour];
		double costWY = params->distanceMatrix[nodeWCour][nodeYCour];

		double routeVGain = costVU + costUX + costXW + costWY;
		double routeVLoss = costVY;
		double routeVBalance = routeVGain - routeVLoss;

	// Check if the move is worth
	if (routeVBalance + routeUBalance > -MY_EPSILON && params->testRoutine == false) return false;
	// TODO: Check LOAD

	// Make the move
	insertNode3(nodeU, nodeV);
	updateRouteData(routeU);
	if (routeU != routeV) updateRouteData(routeV);
	return true;
}

bool RVND::swap11()
{
	// Check if the move is valid
	if (nodeUCour == nodeVPredCour) return false;
	if (nodeUCour == nodeYCour) return false;

	// Inner load check heuristic
	double routeUloadTransfer = params->clients[nodeUCour].demand;
	double routeVloadTransfer = params->clients[nodeVCour].demand;

	if (routeUloadTransfer + routeV->load - routeVloadTransfer > params->vehicleCapacity) return false;
	if (routeVloadTransfer + routeU->load - routeUloadTransfer > params->vehicleCapacity) return false;
	
	// Costs calculations
		// Route U
		double costPredUU = params->distanceMatrix[nodeUPredCour][nodeUCour];
		double costUX = params->distanceMatrix[nodeUCour][nodeXCour];
		double costPredUV = params->distanceMatrix[nodeUPredCour][nodeVCour];
		double costVX = params->distanceMatrix[nodeVCour][nodeXCour];

		double routeUGain = costPredUV + costVX;
		double routeULoss = costPredUU + costUX;
		double routeUBalance = routeUGain - routeULoss;

		// Route V 
		double costPredVV = params->distanceMatrix[nodeVPredCour][nodeVCour];
		double costVY = params->distanceMatrix[nodeVCour][nodeYCour];
		double costPredVU = params->distanceMatrix[nodeVPredCour][nodeUCour];
		double costUY = params->distanceMatrix[nodeUCour][nodeYCour];

		double routeVGain = costPredVU + costUY;
		double routeVLoss = costPredVV + costVY;
		double routeVBalance = routeVGain - routeVLoss;

	// Check if the move is worth
	if (routeVBalance + routeUBalance > -MY_EPSILON && params->testRoutine == false) return false;
	// TODO: Check LOAD

	// Make the move
	swapNode(nodeU, nodeV);
	updateRouteData(routeU);
	if (routeU != routeV) updateRouteData(routeV);
	return true;
}

bool RVND::swap21()
{
	// Check if the move is valid
	if (nodeU->next->isDepot) return false;
	if (nodeUCour == nodeVCour || nodeXCour == nodeVCour) return false;
	//if (nodeXCour == nodeVPredCour) return false;

	// Inner load check heuristic
	double routeUloadTransfer = params->clients[nodeUCour].demand + params->clients[nodeXCour].demand;
	double routeVloadTransfer = params->clients[nodeVCour].demand;

	if (routeUloadTransfer + routeV->load - routeVloadTransfer > params->vehicleCapacity) return false;
	if (routeVloadTransfer + routeU->load - routeUloadTransfer > params->vehicleCapacity) return false;

	// Costs calculations
		// Missing node IDS
		int nodePostXCour = nodeX->next->cour;

		// Common segments
		double costUX = params->distanceMatrix[nodeUCour][nodeXCour];

		// Route U
		double costPredUU = params->distanceMatrix[nodeUPredCour][nodeUCour];
		double costXPostX = params->distanceMatrix[nodeXCour][nodePostXCour];
		double costPredUV = params->distanceMatrix[nodeUPredCour][nodeVCour];
		double costVPostX = params->distanceMatrix[nodeVCour][nodePostXCour];

		double routeUGain = costPredUV + costVPostX;
		double routeULoss = costPredUU + costUX + costXPostX;
		double routeUBalance = routeUGain - routeULoss;

		// Route V 
		double costPredVV = params->distanceMatrix[nodeVPredCour][nodeVCour];
		double costVY = params->distanceMatrix[nodeVCour][nodeYCour];
		double costPredVU = params->distanceMatrix[nodeVPredCour][nodeUCour];
		double costXY = params->distanceMatrix[nodeXCour][nodeYCour];

		double routeVGain = costPredVU + costUX + costXY;
		double routeVLoss = costPredVV + costVY;
		double routeVBalance = routeVGain - routeVLoss;

	// Check if the move is worth
	if (routeVBalance + routeUBalance > -MY_EPSILON && params->testRoutine == false) return false;
	// TODO: Check LOAD

	// Make the move
	swapNode2(nodeU, nodeV);
	updateRouteData(routeU);
	if (routeU != routeV) updateRouteData(routeV);
	return true;
}

bool RVND::swap22()
{
	// Check if the move is valid
	if (nodeU->next->isDepot) return false;
	if (nodeV->next->isDepot) return false;
	if (nodeUCour == nodeVCour || nodeUCour == nodeYCour || nodeVCour == nodeXCour) return false;
	//if (nodeXCour == nodeVPredCour) return false;

	// Inner load check heuristic
	double routeUloadTransfer = params->clients[nodeUCour].demand + params->clients[nodeXCour].demand;
	double routeVloadTransfer = params->clients[nodeVCour].demand + params->clients[nodeYCour].demand;

	if (routeUloadTransfer + routeV->load - routeVloadTransfer > params->vehicleCapacity) return false;
	if (routeVloadTransfer + routeU->load - routeUloadTransfer > params->vehicleCapacity) return false;

	// Costs calculations
		// Missing node IDS
		int nodePostXCour = nodeX->next->cour;
		int nodePostYCour = nodeY->next->cour;

		// Common segments
		double costUX = params->distanceMatrix[nodeUCour][nodeXCour];
		double costVY = params->distanceMatrix[nodeVCour][nodeYCour];

		// Route U
		double costPredUU = params->distanceMatrix[nodeUPredCour][nodeUCour];
		double costXPostX = params->distanceMatrix[nodeXCour][nodePostXCour];
		double costPredUV = params->distanceMatrix[nodeUPredCour][nodeVCour];
		double costYPostX = params->distanceMatrix[nodeYCour][nodePostXCour];

		double routeUGain = costPredUV + costVY + costYPostX;
		double routeULoss = costPredUU + costUX + costXPostX;
		double routeUBalance = routeUGain - routeULoss;

		// Route V 
		double costPredVV = params->distanceMatrix[nodeVPredCour][nodeVCour];
		double costYPostY = params->distanceMatrix[nodeYCour][nodePostYCour];
		double costPredVU = params->distanceMatrix[nodeVPredCour][nodeUCour];
		double costXPostY = params->distanceMatrix[nodeXCour][nodePostYCour];

		double routeVGain = costPredVU + costUX + costXPostY;
		double routeVLoss = costPredVV + costVY + costYPostY;
		double routeVBalance = routeVGain - routeVLoss;

	// Check if the move is worth
	if (routeVBalance + routeUBalance > -MY_EPSILON && params->testRoutine == false) return false;
	// TODO: Check LOAD

	// Make the move
	swapNode22(nodeU, nodeV);
	updateRouteData(routeU);
	if (routeU != routeV) updateRouteData(routeV);
	return true;
}

bool RVND::cross()
{

}

// ------

/*
bool RVND::reinsertion()
{
	return shift10_check();
}
*/

bool RVND::oropt2()
{
	return shift20();
}

bool RVND::oropt3()
{
	return shift30();
}

bool RVND::exchange()
{
	return swap11();
}

bool RVND::twoopt()
{
	return cross();
}

// -------------------
bool RVND::move1()
{
	double costSuppU = params->distanceMatrix[nodeUPredCour][nodeXCour] - params->distanceMatrix[nodeUPredCour][nodeUCour] - params->distanceMatrix[nodeUCour][nodeXCour];
	double costSuppV = params->distanceMatrix[nodeVCour][nodeUCour] + params->distanceMatrix[nodeUCour][nodeYCour] - params->distanceMatrix[nodeVCour][nodeYCour];

	if (costSuppU + costSuppV > -MY_EPSILON) return false;
	if (nodeUCour == nodeYCour) return false;

	insertNode(nodeU, nodeV);
	updateRouteData(routeU);
	if (routeU != routeV) updateRouteData(routeV);
	return true;
}

bool RVND::move4()
{
	double costSuppU = params->distanceMatrix[nodeUPredCour][nodeVCour] + params->distanceMatrix[nodeVCour][nodeXCour] - params->distanceMatrix[nodeUPredCour][nodeUCour] - params->distanceMatrix[nodeUCour][nodeXCour];
	double costSuppV = params->distanceMatrix[nodeVPredCour][nodeUCour] + params->distanceMatrix[nodeUCour][nodeYCour] - params->distanceMatrix[nodeVPredCour][nodeVCour] - params->distanceMatrix[nodeVCour][nodeYCour];

	if (costSuppU + costSuppV > -MY_EPSILON) return false;
	if (nodeUCour == nodeVPredCour || nodeUCour == nodeYCour) return false;

	swapNode(nodeU, nodeV);
	updateRouteData(routeU);
	if (routeU != routeV) updateRouteData(routeV);
	return true;
}


bool RVND::move7()
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

bool RVND::move8()
{
	double cost = params->distanceMatrix[nodeUCour][nodeVCour] + params->distanceMatrix[nodeXCour][nodeYCour] - params->distanceMatrix[nodeUCour][nodeXCour] - params->distanceMatrix[nodeVCour][nodeYCour];

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

void RVND::updateRouteData(Route * myRoute)
{
	int myplace = 0;
	Node * node = myRoute->depot;
	node->position = myplace;
	double mycharge = 0.;
	node->cumulatedLoad = 0.;

	double thisLoad = 0;
	double last1Load = 0; // load of previous node
	double last2Load = 0; // load of 2 previous node
	double sumLast2 = 0; // Sum of last 2 loads
	double sumLast3 = 0; // Sum of last 3 loads

	bool firstIt = true;
	while (!node->isDepot || firstIt)
	{
		firstIt = false;
		myplace++;
		node = node->next;
		node->position = myplace;
		thisLoad = params->clients[node->cour].demand;
		mycharge += thisLoad;
		node->cumulatedLoad = mycharge;

		// Min Max Updaters
		if (thisLoad < myRoute->min1load || myplace == 1) myRoute->min1load = thisLoad;
		if (thisLoad > myRoute->max1load || myplace == 1) myRoute->max1load = thisLoad;

		if (myplace > 1) {
			// Last 2 min and max
			sumLast2 = last1Load + thisLoad;

			if (sumLast2 < myRoute->min2load || myplace == 2) myRoute->min2load = sumLast2;
			if (sumLast2 > myRoute->max2load || myplace == 2) myRoute->max2load = sumLast2;

			if (myplace > 2) {
				// Last 3 min adn max
				sumLast3 = last2Load + sumLast2;

				if (sumLast3 < myRoute->min3load || myplace == 3) myRoute->min3load = sumLast3;
				if (sumLast3 > myRoute->max3load || myplace == 3) myRoute->max3load = sumLast3;

				// Update load memory
				last2Load = last1Load;
			}

			// Update load memory
			last1Load = thisLoad;
		}
	}

	myRoute->load = mycharge;
	myRoute->nbNodes = myplace;

	if (myRoute->depot->next->isDepot) emptyRoutes.insert(myRoute->cour); // If its an empty route, store it
	else emptyRoutes.erase(myRoute->cour); // Otherwise make sure its not part of the empty route vector
}

// -------------------------------------------------------------
// MOVE SWEEPS: search current solution for the best move
// -------------------------------------------------------------
MoveInfo RVND::shift10_sweep()
{
	MoveCheck moveCheck;
	MoveInfo bestMoveInfo;
	bool firstMove = true;

	for (int routeUid = 0; routeUid < routes.size() - 1; routeUid++) {
		Route* rU = &routes[routeUid];
		for (int routeVid = routeUid + 1; routeVid < routes.size(); routeVid++) {
			Route* rV = &routes[routeVid];

			Node* nU = depots[routeUid].next;
			Node* nV = depots[routeVid].next;

			while (!nU->isDepot)
			{
				while (!nV->isDepot)
				{
					int nodeUid = nU->cour;
					int nodeVid = nV->cour;
					prepareNodes(nodeUid, nodeVid);

					moveCheck = shift10_check();
					if (moveCheck.isValid == true && moveCheck.costChange < -MY_EPSILON) {
						if (moveCheck.costChange < -MY_EPSILON || firstMove == true) {
							firstMove = false;
							bestMoveInfo.costChange = moveCheck.costChange;
							bestMoveInfo.nodeUcour = nodeUid;
							bestMoveInfo.nodeVcour = nodeVid;
						}
					}
					nV = nV->next;
				}
				nU = nU->next;
			}
		}
	}

	return bestMoveInfo;
}

// -------------------------------------------------------------
// SOLUTION FUNCTIONS
// -------------------------------------------------------------

void RVND::loadSolution(Solution & mySol)
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

void RVND::exportSolution(Solution & mySol)
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

RVND::RVND(Params * params) : params (params)
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

void RVND::populateInterRouteNL()
{
	// Init list
	interRouteNL = std::vector<int>(INTERROUTE_MOVES);
	for (int i = 0; i < INTERROUTE_MOVES; i++) interRouteNL.push_back(i);

	// Shuffle list
	std::shuffle(interRouteNL.begin(), interRouteNL.end(), params->generator);
}

MoveInfo RVND::getBestInterRouteMove(int moveId)
{
	MoveInfo moveInfo;
	switch (moveId)
	{
		case 0:
			return shift10_sweep();

		default:
			return moveInfo;
	}
}

bool RVND::doInterMoveOnCurrentSolution(int moveId, MoveInfo & moveInfo)
{
	prepareNodes(moveInfo.nodeUcour, moveInfo.nodeVcour);

	switch (moveId)
	{
	case 0:
		return shift10_do();

	default:
		return false;
	}
}

 
