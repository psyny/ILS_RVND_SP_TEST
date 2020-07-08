#include "RVND.h" 
// ----------------------------------------------------------------------------------
// This first segment is here to test route operations

// Function to check if the moves are working as intended
void RVND::runTests(Solution& mySol, std::string exportFile)
{
	std::cout << "Running tests..." << std::endl;

	// Common for every test
	loadSolution(mySol);
	
	std::vector < std::string > results = std::vector < std::string >();
	params->testRoutine = true;
	routesToString("Initial State: ", results);

	std::cout << "Pronted Initial State..." << std::endl;

	// Run main algorithm
	if (false) {
		run(mySol);

		routesToString("Final State: ", results);

		exportTestToFile(results, exportFile);
		return;
	}

	// Random tests
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

	// Cross
	nodeUid = 4;
	nodeVid = 8;
	prepareNodes(nodeUid, nodeVid);
	moveCheck = cross_check();
	if (moveCheck.isValid == true) cross_do();
	routesToString("Cross, " + std::to_string(nodeUid) + " to " + std::to_string(nodeVid) + ": ", results);

	// Fill First Route and Test other shifts

	// Shift (2,0)
	nodeUid = 7;
	nodeVid = 3;
	prepareNodes(nodeUid, nodeVid);
	moveCheck = shift20_check();
	if (moveCheck.isValid == true) shift20_do();
	routesToString("Shift(2,0), " + std::to_string(nodeUid) + " to " + std::to_string(nodeVid) + ": ", results);

	// Shift (3,0)
	nodeUid = 8;
	nodeVid = 3;
	prepareNodes(nodeUid, nodeVid);
	moveCheck = shift30_check();
	if (moveCheck.isValid == true) shift30_do();
	routesToString("Shift(3,0), " + std::to_string(nodeUid) + " to " + std::to_string(nodeVid) + ": ", results);


	// ---------------------------- Intra-Route Moves
	results.push_back("Intra-Route Moves ------------------------------------------------");
	results.push_back("");

	// Exchange 
	nodeUid = 3;
	nodeVid = 9;
	prepareNodes(nodeUid, nodeVid);
	moveCheck = exchange_check();
	if (moveCheck.isValid == true) exchange_do();
	routesToString("Exchange, " + std::to_string(nodeUid) + " to " + std::to_string(nodeVid) + ": ", results);

	// 2-Opt 
	nodeUid = 2;
	nodeVid = 3;
	prepareNodes(nodeUid, nodeVid);
	moveCheck = twoopt_check();
	if (moveCheck.isValid == true) twoopt_do();
	routesToString("2 Opt, " + std::to_string(nodeUid) + " to " + std::to_string(nodeVid) + ": ", results);

	// Exchange 
	nodeUid = 8;
	nodeVid = 9;
	prepareNodes(nodeUid, nodeVid);
	moveCheck = exchange_check();
	if (moveCheck.isValid == true) exchange_do();
	routesToString("Exchange, " + std::to_string(nodeUid) + " to " + std::to_string(nodeVid) + ": ", results);

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
	/*
	MoveInfo bestMoveInfo;
	bestMoveInfo = shift10_sweep();
	results.push_back("Best Move Info: ");
	results.push_back("CostChange: " + std::to_string(bestMoveInfo.costChange) );
	results.push_back("NodeUid: " + std::to_string(bestMoveInfo.nodeUcour) );
	results.push_back("NodeVid: " + std::to_string(bestMoveInfo.nodeVcour) );
	*/

	// ---------------------------- Export to external file
	exportTestToFile(results, exportFile);
}


void RVND::prepareNodes(int nodeIdU, int nodeIdV, bool isUdepot, bool isVdepot)
{	
	// NODE U
	if (isUdepot == true) {
		nodeU = (&routes[nodeIdU])->depot;
	}
	else {
		nodeU = &clients[nodeIdU];
	}
	setLocalVariablesRouteU();

	// NODE V
	if (isVdepot == true) {
		nodeV = (&routes[nodeIdV])->depot;
	}
	else {
		nodeV = &clients[nodeIdV];
	}
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
		while (!node->isDepot)
		{
			routeString += " " + std::to_string(node->cour);
			node = node->next;
		}
		routeString += " 0 ";

		// Attach more info ( Cost and Load )
		routeString += "\t[";

		costString = std::to_string(node->route->totalCost);
		costString = costString.substr(0, costString.find(".") + 3);
		routeString += "cost: " + costString;
		
		costString = std::to_string(node->route->load);
		costString = costString.substr(0, costString.find(".") + 0);
		routeString += "  \tload: " + costString;

		costString = std::to_string(node->route->nbNodes);
		costString = costString.substr(0, costString.find(".") + 0);
		routeString += "  \tsize: " + costString;

		routeString += "]";

		// Route cost
		results.push_back(routeString);

		// Update total cost
		totalCost += node->route->totalCost;
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

	// Do a small intra route optimization on entire solution
	if (neighbStatus->hadInitialOptimization == false) {
		neighbStatus->hadInitialOptimization = true;
		doIntraRouteOptimizationOnSolution();
	}

	// Init inter route NL
	populateInterRouteNL();

	// Main Loop
	while (interRouteNL.size() > 0)
	{
		// Choose a random move to create a neighborhoood (the list init is random)
		int moveId = interRouteNL.back();

		// Find the best solution parameters on the neighborhood
		MoveInfo moveInfo = getBestInterRouteMove(moveId);

		// Check if the move is good enough
		if (moveInfo.costChange <= -MY_EPSILON)
		{
			// Do the move ( ADSs will be updated)
			doInterMoveOnCurrentSolution(moveId, moveInfo);

			// Intra Route Search ---------------------
			// Much like inter route search, but simplier xxx
			int routeU = clients[moveInfo.nodeUcour].route->cour;
			int routeV = 0;
			if (moveInfo.isVdepot == true) routeV = moveInfo.nodeVcour;
			else routeV = clients[moveInfo.nodeVcour].route->cour;

			doIntraRouteOptimizationOnRoute(routeU);
			doIntraRouteOptimizationOnRoute(routeV);

			// Reset NL
			populateInterRouteNL();
		}
		else
		{
			// Remove move from NL
			interRouteNL.pop_back();
		}

		// UPDATE ADS
	}

	// Try to empty routes

	exportSolution(mySol);
}

void RVND::perturb(Solution& mySol, bool solutionWasImproved)
{
	loadSolution(mySol);

	// UPDATE ADS
	// WHY?

	// Init inter route NL
	populatePerturbRouteNL();

	// Choose a random move to create a neighborhoood (the list init is random)
	int moveId = perturbRouteNL.back();

	// Min placement location can vary with movement type (some can be inserted at a depot, some cant)
	int minTarget = 0;
	if (moveId == 0) minTarget = 0;
	else minTarget = 1;

	// Random moves
	int maxpertubations = PERTURBROUTE_MAX;
	if (solutionWasImproved == true) maxpertubations = ((int)maxpertubations / 2);
	int minpertubations = 1;
	if (minpertubations > maxpertubations) minpertubations = maxpertubations;

	int numberofMoves = 0;
	if (minpertubations < maxpertubations) {
		std::uniform_int_distribution<int> dist_moves(minpertubations, maxpertubations);
		numberofMoves = dist_moves(params->generator);
	}
	else {
		numberofMoves = minpertubations;
	}


	int failedMoves = 0;
	int moveNum = 0;

	while (moveNum < numberofMoves && failedMoves < PERTURBROUTE_MAXFAILS) {
		// Get two random routes
		std::uniform_int_distribution<int> dist_routeU(0, routes.size() - 1);
		int routeUid = dist_routeU(params->generator);

		std::uniform_int_distribution<int> dist_routeV(0, routes.size() - 1);
		int routeVid = dist_routeV(params->generator);

		// Check if routes are valid
		if (
			routeUid == routeVid 
			|| routes[routeUid].nbNodes < 2
			|| routes[routeVid].nbNodes < 2
		) {
			failedMoves++;
			continue;
		}

		// Get node order in the route to get the node id
		int node1OriginNum = 1;
		if (routes[routeUid].nbNodes - 1 > 1) {
			std::uniform_int_distribution<int> rand_node1Origin(1, routes[routeUid].nbNodes - 1);
			node1OriginNum = rand_node1Origin(params->generator);
		}
		int node2DestinyNum = 0;

		if (moveId == 0) {
			std::uniform_int_distribution<int> rand_node2Destiny(0, routes[routeUid].nbNodes - 1);
			node2DestinyNum = rand_node2Destiny(params->generator);

			if (node1OriginNum == node2DestinyNum) {
				node2DestinyNum--;
			}
		}

		std::uniform_int_distribution<int> rand_node1Destiny(minTarget, routes[routeVid].nbNodes - 1);
		int node1DestinyNum = minTarget;
		if (routes[routeVid].nbNodes - 1 > minTarget) node1DestinyNum = rand_node1Destiny(params->generator);
		int node2OriginNum = 0;

		if (moveId == 0) {
			
			node2OriginNum = 1;
			if (routes[routeVid].nbNodes - 1 > 1) {
				std::uniform_int_distribution<int> rand_node2Origin(1, routes[routeVid].nbNodes - 1);
				node2OriginNum = rand_node2Origin(params->generator);
			}

			//std::cout << "ID: " << node2OriginNum << std::endl;

			if (node2OriginNum == node1DestinyNum) {
				node1DestinyNum--;
			}
		}

		#if RVND_DEBUG == true
			std::cout << "Move ID: " << moveId << " | Nodes: U: " << routes[routeUid].nbNodes << " V: " << routes[routeVid].nbNodes << std::endl;
			std::cout
				<< "  No.: O1: " << node1OriginNum << " D1: " << node1DestinyNum
				<< " O2: " << node2OriginNum << " D2: " << node2DestinyNum
				<< " " << std::endl;
		#endif


		// Search for the nodes
		Node* node;
		int node1OriginId = -1;
		int node1DestinyId = -1;
		int node2OriginId = -1;
		int node2DestinyId = -1;
		int nodeNum;
		bool node1DestinyIsDepot = false;
		bool node2DestinyIsDepot = false;

		if (node1OriginNum == 0)  node1OriginId = 0;
		if (node2OriginNum == 0)  node2OriginId = 0;
		if (node1DestinyNum == 0) {
			node1DestinyId = 0;
			node1DestinyIsDepot = true;
		}
		if (node2DestinyNum == 0) {
			node2DestinyId = 0;
			node2DestinyIsDepot = true;
		}

		node = depots[routeUid].next;
		nodeNum = 1;
		while (node1OriginId < 0 || node2DestinyId < 0 ) {
			#if RVND_DEBUG == true
				if (node->isDepot) {
					std::cout << "IS DEPOT U: order: " << nodeNum << " | id: " << node->cour  <<  std::endl;
				}
			#endif

			if (nodeNum == node1OriginNum) node1OriginId = node->cour;
			else if (nodeNum == node2DestinyNum) node2DestinyId = node->cour;

			nodeNum++;
			node = node->next;
		}

		node = depots[routeVid].next;
		nodeNum = 1;
		while (node2OriginId < 0 || node1DestinyId < 0) {
			#if RVND_DEBUG == true
				if (node->isDepot) {
					std::cout << "IS DEPOT V: order: " << nodeNum << " | id: " << node->cour << std::endl;
				}
			#endif

			if (nodeNum == node2OriginNum) node2OriginId = node->cour;
			else if (nodeNum == node1DestinyNum) node1DestinyId = node->cour;

			nodeNum++;
			node = node->next;
		}

		#if RVND_DEBUG == true
			std::cout
				<< "  Id.: O1: " << node1OriginId << " D1: " << node1DestinyId
				<< " O2: " << node2OriginId << " D2: " << node2DestinyId
				<< " " << std::endl;
		#endif

		// Apply picked move
		MoveCheck moveCheck;
		isPerturb = true;
		switch (moveId) {
		case 0:
			// Shift(1,1)
			prepareNodes(node1OriginId, node1DestinyId, false, node1DestinyIsDepot);
			moveCheck = shift10_check();
			if (moveCheck.isValid) {
				shift10_do();

				prepareNodes(node2OriginId, node2DestinyId, false, node2DestinyIsDepot);
				moveCheck = shift10_check();
				if (moveCheck.isValid) {
					shift10_do();
					moveNum;
				}
				else {
					failedMoves++;
					continue;
				}
			}
			else
			{
				failedMoves++;
				continue;
			}
			moveNum++;
			break;

		default:
			// Swap(1,1)
			prepareNodes(node1OriginId, node1DestinyId);
			moveCheck = swap11_check();
			if (moveCheck.isValid) {
				swap11_do();
			}
			else
			{
				failedMoves++;
				continue;
			}
			moveNum++;
			break;
		}

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

void RVND::swapNode11(Node * U, Node * V)
{
	if (U->next == V)
	{
		// Swap region contour nodes
		U->prev->next = V;
		V->next->prev = U;

		// Swap nodes positions
		V->prev = U->prev;
		U->next = V->next;

		U->prev = V;
		V->next = U;
	}
	else if (V->next == U)
	{
		// Swap region contour nodes
		V->prev->next = U;
		U->next->prev = V;

		// Swap nodes positions
		U->prev = V->prev;
		V->next = U->next;

		V->prev = U;
		U->next = V;
	}
	else
	{
		// Swap region contour nodes
		U->prev->next = V;
		U->next->prev = V;

		V->prev->next = U;
		V->next->prev = U;

		// Swap nodes positions
		Node* Uprev = U->prev;
		Node* Unext = U->next;

		U->prev = V->prev;
		U->next = V->next;

		V->prev = Uprev;
		V->next = Unext;
	}

	// Updating routes
	Route* RU = U->route;
	U->route = V->route;
	V->route = RU;
}

void RVND::swapNode21(Node* U, Node* V)
{
	// Other related nodes
	Node* X = U->next;

	if (X->next == V)
	{
		// Swap region contour nodes
		U->prev->next = V;
		V->next->prev = X;

		// Swap nodes positions
		V->prev = U->prev;
		X->next = V->next;

		U->prev = V;
		V->next = X;
	}
	else if (V->next == U)
	{
		// Swap region contour nodes
		V->prev->next = U;
		X->next->prev = V;

		// Swap nodes positions
		U->prev = V->prev;
		V->next = X->next;

		V->prev = X;
		X->next = V;
	}
	else
	{
		// Swap region contour nodes
		U->prev->next = V;
		X->next->prev = V;

		V->prev->next = U;
		V->next->prev = X;

		// Swap nodes positions
		Node* Uprev = U->prev;
		Node* Xnext = X->next;

		U->prev = V->prev;
		X->next = V->next;

		V->prev = Uprev;
		V->next = Xnext;
	}

	// Updating routes
	Route* RU = U->route;
	U->route = V->route;
	X->route = V->route;
	V->route = RU;
}

void RVND::swapNode22(Node* U, Node* V)
{
	// Other related nodes
	Node* X = U->next;
	Node* Y = V->next;

	if (X->next == V)
	{
		// Swap region contour nodes
		U->prev->next = V;
		Y->next->prev = X;

		// Swap nodes positions
		V->prev = U->prev;
		X->next = Y->next;

		U->prev = Y;
		Y->next = U;
	}
	else if (Y->next == U)
	{
		// Swap region contour nodes
		V->prev->next = U;
		X->next->prev = Y;

		// Swap nodes positions
		U->prev = V->prev;
		Y->next = X->next;

		V->prev = X;
		X->next = V;
	}
	else
	{
		// Swap region contour nodes
		U->prev->next = V;
		X->next->prev = Y;

		V->prev->next = U;
		Y->next->prev = X;

		// Swap nodes positions
		Node* Uprev = U->prev;
		Node* Xnext = X->next;

		U->prev = V->prev;
		X->next = Y->next;

		V->prev = Uprev;
		Y->next = Xnext;
	}

	// Updating routes
	Route* RU = U->route;
	U->route = V->route;
	X->route = V->route;
	V->route = RU;
	Y->route = RU;
}

void RVND::crossNode(Node* U, Node* V)
{
	// Other related nodes
	Node* X = U->next;
	Node* Y = V->next;

	// Node swaps
	U->next = Y;
	V->next = X;

	X->prev = V;
	Y->prev = U;

	// Depot Adjustments
	Node* UstartDepot = U->route->depot;
	Node* UendDepot = UstartDepot->prev;
	Node* Ulast = UendDepot->prev;
	Node* VstartDepot = V->route->depot;
	Node* VendDepot = VstartDepot->prev;
	Node* Vlast = VendDepot->prev;

	Vlast->next = UendDepot;
	Ulast->next = VendDepot;

	UendDepot->prev = Vlast;
	VendDepot->prev = Ulast;
}

void RVND::crossNodeSameRoute(Node* U, Node* V)
{
	// Other related nodes
	Node* X = U->next;
	Node* Y = V->next;

	// We need to go throu a loop to invert the route... Heavy? Yes...
	Node* currentNode = V;
	Node* nextNode;
	Node* prevNode = 0;

	//std::cout << " ssssss " << std::endl; 
	//std::cout << U->route->cour << " | " << U->position << " | " << V->position << std::endl; 

	//std::cout << " ------ " << std::endl;
	/*
	Node* tNode = U->route->depot->next;
	while (!tNode->isDepot)
	{
		//std::cout << " " << tNode->cour;
		tNode = tNode->next;
	}
	*/
	//std::cout << std::endl;

	//std::cout << " ------ " << std::endl;

	while (currentNode->cour != X->cour) {
		//std::cout << currentNode->cour << std::endl;

		nextNode = currentNode->prev;

		currentNode->next = nextNode;
		// Only updates above first iteration
		if (currentNode->cour != V->cour) {
			currentNode->prev = prevNode;
		}
		
		// Update references
		prevNode = currentNode;
		currentNode = nextNode;
	}

	//std::cout << " eee " << std::endl; 

	// Fix contour references
	//std::cout << " prevNode: " << prevNode << std::endl; 
	X->prev = prevNode;

	U->next = V;
	V->prev = U;

	X->next = Y;
	Y->prev = X;

	//std::cout << " eeeeee " << std::endl; 
}

// --------------------------------------------------- MOVES
// Insert given Node U and its next Node X after the given Node V
MoveCheck RVND::shift10_check()
{
	MoveCheck moveCheck;

	// Check if the move will result in changes
	if (nodeUCour == nodeVCour) return moveCheck; // Same place
	if (nodeUCour == nodeYCour) return moveCheck; // Same place

	// Check if the move is valid (will not break routes integrity)
	if (nodeUCour == 0) return moveCheck; // Cannot relocate the depot

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
MoveCheck RVND::shift20_check()
{
	MoveCheck moveCheck;

	// Check if the move will result in changes
	if (nodeUCour == nodeVCour) return moveCheck; // Same place
	if (nodeUCour == nodeYCour) return moveCheck; // Same place

	// Check if the move is valid (will not break routes integrity)
	if (nodeUCour == 0) return moveCheck; // Cannot relocate the depot
	if (nodeU->next->isDepot) return moveCheck; // Cannot relocate the depot
	if (routeU == routeV) {
		if (nodeXCour == nodeVCour) return moveCheck;
	}

	// Inner load check heuristic
	double routeUloadTransfer = params->clients[nodeUCour].demand + params->clients[nodeXCour].demand;
	if (routeUloadTransfer + routeV->load > params->vehicleCapacity) return moveCheck;

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

	// Return Struct
	moveCheck.isValid = true;
	moveCheck.costChange = routeUBalance + routeVBalance;
	return moveCheck;
}

bool RVND::shift20_do() {
	// Make the move
	insertNode2(nodeU, nodeV);
	updateRouteData(routeU);
	if (routeU != routeV) updateRouteData(routeV);
	return true;
}

// Insert given Node U and its two next Nodes X and W after the given Node V
MoveCheck RVND::shift30_check()
{
	MoveCheck moveCheck;

	// Check if the move will result in changes
	if (nodeUCour == nodeVCour) return moveCheck; // Same place
	if (nodeUCour == nodeYCour) return moveCheck; // Same place

	// Check if the move is valid (will not break routes integrity)
	if (nodeUCour == 0) return moveCheck; // Cannot relocate the depot
	if (nodeU->next->isDepot) return moveCheck; // Cannot relocate the depot
	if (nodeU->next->next->isDepot) return moveCheck; // Cannot relocate the depot
	if (routeU == routeV) {
		if (nodeXCour == nodeVCour) return moveCheck;
		if (nodeX->next->cour == nodeVCour) return moveCheck;
	}

	// Inner load check heuristic
	int nodeWCour = nodeX->next->cour; // Node W = Node X -> Next ( the last node on string to be transfered )
	double routeUloadTransfer = params->clients[nodeUCour].demand
		+ params->clients[nodeXCour].demand
		+ params->clients[nodeWCour].demand;
	if (routeUloadTransfer + routeV->load > params->vehicleCapacity) return moveCheck;

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

	// Return Struct
	moveCheck.isValid = true;
	moveCheck.costChange = routeUBalance + routeVBalance;
	return moveCheck;
}

bool RVND::shift30_do() {
	// Make the move
	insertNode3(nodeU, nodeV);
	updateRouteData(routeU);
	if (routeU != routeV) updateRouteData(routeV);
	return true;
}

MoveCheck RVND::swap11_check()
{
	MoveCheck moveCheck;

	// Check if the move will result in changes
	if (nodeUCour == nodeVCour) return moveCheck; // Same place

	// Check if the move is valid (will not break routes integrity)
	if (nodeUCour == 0) return moveCheck; // Cannot relocate the depot
	if (nodeVCour == 0) return moveCheck; // Cannot relocate the depot

	// Inner load check heuristic
	double routeUloadTransfer = params->clients[nodeUCour].demand;
	double routeVloadTransfer = params->clients[nodeVCour].demand;

	if (routeUloadTransfer + routeV->load - routeVloadTransfer > params->vehicleCapacity) return moveCheck;
	if (routeVloadTransfer + routeU->load - routeUloadTransfer > params->vehicleCapacity) return moveCheck;
	
	// Costs calculations
	double routeUBalance = 0;
	double routeVBalance = 0;
	if (nodeXCour == nodeVCour || nodeYCour == nodeUCour)
	{
		// Swaping adjacent nodes, different cost
		double routeLoss = 0;
		double routeGain = 0;

		if (nodeU->position < nodeV->position) {
			double costPredUU = params->distanceMatrix[nodeUPredCour][nodeUCour];
			double costVY = params->distanceMatrix[nodeVCour][nodeYCour];
			double costPredUV = params->distanceMatrix[nodeUPredCour][nodeVCour];
			double costUY = params->distanceMatrix[nodeUCour][nodeYCour];

			routeLoss = costPredUU + costVY;
			routeGain = costPredUV + costUY;
		}
		else {
			double costPredVV = params->distanceMatrix[nodeVPredCour][nodeVCour];
			double costUX = params->distanceMatrix[nodeUCour][nodeXCour];
			double costPredVU = params->distanceMatrix[nodeVPredCour][nodeUCour];
			double costVX = params->distanceMatrix[nodeVCour][nodeXCour];

			routeLoss = costPredVV + costUX;
			routeGain = costPredVU + costVX;
		}
		routeUBalance = routeGain - routeLoss;
		routeVBalance = 0;
	}
	else
	{
		// Route U
		double costPredUU = params->distanceMatrix[nodeUPredCour][nodeUCour];
		double costUX = params->distanceMatrix[nodeUCour][nodeXCour];
		double costPredUV = params->distanceMatrix[nodeUPredCour][nodeVCour];
		double costVX = params->distanceMatrix[nodeVCour][nodeXCour];

		double routeUGain = costPredUV + costVX;
		double routeULoss = costPredUU + costUX;
		routeUBalance = routeUGain - routeULoss;

		// Route V 
		double costPredVV = params->distanceMatrix[nodeVPredCour][nodeVCour];
		double costVY = params->distanceMatrix[nodeVCour][nodeYCour];
		double costPredVU = params->distanceMatrix[nodeVPredCour][nodeUCour];
		double costUY = params->distanceMatrix[nodeUCour][nodeYCour];

		double routeVGain = costPredVU + costUY;
		double routeVLoss = costPredVV + costVY;
		routeVBalance = routeVGain - routeVLoss;
	}

	// Return Struct
	moveCheck.isValid = true;
	moveCheck.costChange = routeUBalance + routeVBalance;
	return moveCheck;
}

bool RVND::swap11_do()
{
	swapNode11(nodeU, nodeV);
	updateRouteData(routeU);
	if (routeU != routeV) updateRouteData(routeV);
	return true;
}

MoveCheck RVND::swap21_check()
{
	MoveCheck moveCheck;

	// Check if the move will result in changes
	if (nodeUCour == nodeVCour) return moveCheck; // Same place

	// Check if the move is valid (will not break routes integrity)
	if (nodeUCour == 0) return moveCheck; // Cannot relocate the depot
	if (nodeVCour == 0) return moveCheck; // Cannot relocate the depot
	if (nodeX->isDepot) return moveCheck;
	if (nodeXCour == nodeVCour) return moveCheck;

	// Inner load check heuristic
	double routeUloadTransfer = params->clients[nodeUCour].demand + params->clients[nodeXCour].demand;
	double routeVloadTransfer = params->clients[nodeVCour].demand;

	if (routeUloadTransfer + routeV->load - routeVloadTransfer > params->vehicleCapacity) return moveCheck;
	if (routeVloadTransfer + routeU->load - routeUloadTransfer > params->vehicleCapacity) return moveCheck;

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

	// Return Struct
	moveCheck.isValid = true;
	moveCheck.costChange = routeUBalance + routeVBalance;
	return moveCheck;
}

bool RVND::swap21_do() {
	// Make the move
	swapNode21(nodeU, nodeV);
	updateRouteData(routeU);
	if (routeU != routeV) updateRouteData(routeV);
	return true;
}

MoveCheck RVND::swap22_check()
{
	MoveCheck moveCheck;

	// Check if the move will result in changes
	if (nodeUCour == nodeVCour) return moveCheck; // Same place

	// Check if the move is valid (will not break routes integrity)
	if (nodeUCour == 0) return moveCheck; // Cannot relocate the depot
	if (nodeVCour == 0) return moveCheck; // Cannot relocate the depot
	if (nodeX->isDepot) return moveCheck;
	if (nodeY->isDepot) return moveCheck;
	if (nodeXCour == nodeVCour) return moveCheck;
	if (nodeYCour == nodeUCour) return moveCheck;

	// Inner load check heuristic
	double routeUloadTransfer = params->clients[nodeUCour].demand + params->clients[nodeXCour].demand;
	double routeVloadTransfer = params->clients[nodeVCour].demand + params->clients[nodeYCour].demand;

	if (routeUloadTransfer + routeV->load - routeVloadTransfer > params->vehicleCapacity) return moveCheck;
	if (routeVloadTransfer + routeU->load - routeUloadTransfer > params->vehicleCapacity) return moveCheck;

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

	// Return Struct
	moveCheck.isValid = true;
	moveCheck.costChange = routeUBalance + routeVBalance;
	return moveCheck;
}

bool RVND::swap22_do() {
	// Make the move
	swapNode22(nodeU, nodeV);
	updateRouteData(routeU);
	if (routeU != routeV) updateRouteData(routeV);
	return true;
}

MoveCheck RVND::cross_check()
{
	MoveCheck moveCheck;

	// Check if the move will result in changes
	if (nodeUCour == nodeVCour) return moveCheck; // Same place
	if (nodeX->isDepot && nodeY->isDepot) return moveCheck; // Same place

	// Check if the move is valid (will not break routes integrity)
	if (nodeUCour == 0) return moveCheck; // Cannot relocate the depot
	if (nodeVCour == 0) return moveCheck; // Cannot relocate the depot

	// Inner load check heuristic
	double routeUpreUload = nodeU->cumulatedLoad;
	double routeUposXload = routeU->load - nodeX->cumulatedLoad + params->clients[nodeXCour].demand;
	double routeVpreVload = nodeV->cumulatedLoad;
	double routeVposYload = routeV->load - nodeY->cumulatedLoad + params->clients[nodeYCour].demand;

	if (routeUpreUload + routeVposYload > params->vehicleCapacity) return moveCheck;
	if (routeVpreVload + routeUposXload > params->vehicleCapacity) return moveCheck;

	// Costs calculations
		// Route U
		double routeCostDepotToU = nodeU->cumulatedCost;
		double routeCostXtoDepot = routeU->totalCost - nodeX->cumulatedCost;
		double costUX = params->distanceMatrix[nodeUCour][nodeXCour];
		double costUY = params->distanceMatrix[nodeUCour][nodeYCour];

		// Route V
		double routeCostDepotToV = nodeV->cumulatedCost;
		double routeCostYtoDepot = routeV->totalCost - nodeY->cumulatedCost;
		double costVY = params->distanceMatrix[nodeVCour][nodeYCour];
		double costVX = params->distanceMatrix[nodeVCour][nodeXCour];

		// Gains and Losses
		double routeUGain = costUY + routeCostYtoDepot;
		double routeULoss = costUX + routeCostXtoDepot;
		double routeUBalance = routeUGain - routeULoss;

		double routeVGain = costVX + routeCostXtoDepot;
		double routeVLoss = costVY + routeCostYtoDepot;
		double routeVBalance = routeVGain - routeVLoss;

	// Return Struct
	moveCheck.isValid = true;
	moveCheck.costChange = routeUBalance + routeVBalance;
	return moveCheck;
}

bool RVND::cross_do()
{
	// Make the move
	crossNode(nodeU, nodeV);
	updateRouteData(routeU, true);
	if (routeU != routeV) updateRouteData(routeV, true);
	return true;
}

// -------------------------------------------------------------
// MOVES: Intra-Route
// -------------------------------------------------------------

MoveCheck RVND::reinsertion_check()
{
	return shift10_check();
}

bool RVND::reinsertion_do()
{
	return shift10_do();
}

MoveCheck RVND::oropt2_check()
{
	return shift20_check();
}

bool RVND::oropt2_do()
{
	return shift20_do();
}

MoveCheck RVND::oropt3_check()
{
	return shift30_check();
}

bool RVND::oropt3_do()
{
	return shift30_do();
}

MoveCheck RVND::exchange_check()
{
	return swap11_check();
}

bool RVND::exchange_do()
{
	return swap11_do();
}

MoveCheck RVND::twoopt_check()
{
	MoveCheck moveCheck;

	// Check if the move will result in changes
	if (nodeUCour == nodeVCour) return moveCheck; // Same place

	// Check if the move is valid (will not break routes integrity)
	if (nodeUCour == 0) return moveCheck; // Cannot relocate the depot
	if (nodeVCour == 0) return moveCheck; // Cannot relocate the depot
	if (nodeX->isDepot) return moveCheck; // Edge limit
	if (nodeY->isDepot) return moveCheck; // Edge limit

	// Move Checks by move definition
	if (nodeU->position > nodeV->position) return moveCheck; // Out of order
	if (nodeXCour == nodeVCour) return moveCheck; // Adjacent

	// Costs calculations
		double costUX = params->distanceMatrix[nodeUCour][nodeXCour];
		double costVY = params->distanceMatrix[nodeVCour][nodeYCour];
		double costUV = params->distanceMatrix[nodeUCour][nodeVCour];
		double costXY = params->distanceMatrix[nodeXCour][nodeYCour];

		// Gains and Losses
		double gain = costUV + costXY;
		double loss = costUX + costVY;
		double balance = gain - loss;

	// Return Struct
	moveCheck.isValid = true;
	moveCheck.costChange = balance;
	return moveCheck;
}

bool RVND::twoopt_do()
{
	// Make the move
	crossNodeSameRoute(nodeU, nodeV);
	updateRouteData(routeU);
	return true;
}

// -------------------

void RVND::updateRouteData(Route * myRoute, bool updateNodeRouteInfo, bool updateStatus)
{
	int myplace = 0;
	Node * node = myRoute->depot;
	Node* prevNode;
	node->position = myplace;
	double mycharge = 0.;
	node->cumulatedLoad = 0.;
	node->cumulatedCost = 0.;

	double thisLoad = 0;
	double currentCost = 0;
	double last1Load = 0; // load of previous node
	double sumLast2 = 0; // Sum of last 2 loads

	bool firstIt = true;
	while (!node->isDepot || firstIt)
	{
		firstIt = false;
		myplace++;
		prevNode = node;
		node = node->next;
		node->position = myplace;
		thisLoad = params->clients[node->cour].demand;
		mycharge += thisLoad;
		node->cumulatedLoad = mycharge;

		if (updateNodeRouteInfo && !node->isDepot) node->route = myRoute;

		// Cost updater
		currentCost += params->distanceMatrix[prevNode->cour][node->cour];
		node->cumulatedCost = currentCost;

		// Min Max Updaters
		if (thisLoad < myRoute->min1load || myplace == 1) myRoute->min1load = thisLoad;
		if (thisLoad > myRoute->max1load || myplace == 1) myRoute->max1load = thisLoad;

		if (myplace > 1) {
			// Last 2 min and max
			sumLast2 = last1Load + thisLoad;

			if (sumLast2 < myRoute->min2load || myplace == 2) myRoute->min2load = sumLast2;
			if (sumLast2 > myRoute->max2load || myplace == 2) myRoute->max2load = sumLast2;

			// Update load memory
			last1Load = thisLoad;
		}
	}

	// Last adjustment
	currentCost += params->distanceMatrix[prevNode->cour][node->cour];

	// Route updaters
	myRoute->load = mycharge;
	myRoute->totalCost = currentCost;
	myRoute->nbNodes = myplace;

	if (myRoute->depot->next->isDepot) emptyRoutes.insert(myRoute->cour); // If its an empty route, store it
	else emptyRoutes.erase(myRoute->cour); // Otherwise make sure its not part of the empty route vector

	// Update ADS to check route changes
	if (updateStatus == true) {
		neighbStatus->routeRelatedToImprove(myRoute->cour);
	}
}

// -------------------------------------------------------------
// INTER MOVE SWEEPS: search current solution for the best move
// -------------------------------------------------------------
MoveInfo RVND::shift10_sweep()
{
	MoveCheck moveCheck;
	MoveInfo bestMoveInfo;
	bool firstMove = true;
	int moveId = 0;

	for (int routeUid = 0; routeUid < routes.size() - 1; routeUid++) {
		Route* rU = &routes[routeUid];
		if (rU->nbNodes < 2) {
			neighbStatus->setMoveRouteState(moveId, routeUid, false);
			continue;
		}
		bool hadAnyImprovement = false;

		for (int routeVid = 0; routeVid < routes.size(); routeVid++) {
			// ADS neighborhood status check heuristic
			if (neighbStatus->worthMoveBetween(moveId, routeUid, routeVid) == false) continue;

			Route* rV = &routes[routeVid];

			// Heuristic Cuts
			if (routeVid == routeUid) continue;
			if (rU->min1load + rV->load > params->vehicleCapacity) continue;

			// ----
			Node* nU = depots[routeUid].next;
			Node* nV = &depots[routeVid];
			bool depotTried = false;

			while (!nU->isDepot)
			{
				int nodeUid = nU->cour;

				// Heuristic Cuts
				if (params->clients[nodeUid].demand + rV->load > params->vehicleCapacity) {
					nU = nU->next;
					continue;
				}

				// Find the best position on route V
				while (!nV->isDepot || !depotTried)
				{
					depotTried = true;

					int nodeVid = nV->cour;
					if (nV->isDepot) nodeVid = nV->route->cour;
	
					prepareNodes(nodeUid, nodeVid, false, nV->isDepot);

					moveCheck = shift10_check();
					if (moveCheck.isValid == true && moveCheck.costChange < -MY_EPSILON) {
						if (moveCheck.costChange < bestMoveInfo.costChange || firstMove == true) {
							hadAnyImprovement = true;
							firstMove = false;
							bestMoveInfo.costChange = moveCheck.costChange;
							bestMoveInfo.nodeUcour = nodeUid;
							bestMoveInfo.nodeVcour = nodeVid;
							bestMoveInfo.isVdepot = nV->isDepot;
						}
					}
					nV = nV->next;
				}
				nU = nU->next;
			}
		}

		if (hadAnyImprovement == false) neighbStatus->setMoveRouteState(moveId, routeUid, false);
	}

	return bestMoveInfo;
}

MoveInfo RVND::shift20_sweep()
{
	MoveCheck moveCheck;
	MoveInfo bestMoveInfo;
	bool firstMove = true;
	int moveId = 1;

	for (int routeUid = 0; routeUid < routes.size() - 1; routeUid++) {
		Route* rU = &routes[routeUid];
		if (rU->nbNodes < 2) {
			neighbStatus->setMoveRouteState(moveId, routeUid, false);
			continue;
		}
		bool hadAnyImprovement = false;

		for (int routeVid = 0; routeVid < routes.size(); routeVid++) {
			// ADS neighborhood status check heuristic
			if (neighbStatus->worthMoveBetween(moveId, routeUid, routeVid) == false) continue;

			Route* rV = &routes[routeVid];

			// Heuristic Cuts
			if (routeVid == routeUid) continue;
			if (rU->min2load + rV->load > params->vehicleCapacity) continue;

			// -------
			Node* nU = depots[routeUid].next;
			Node* nV = &depots[routeVid];
			bool depotTried = false;

			while (!nU->isDepot)
			{
				int nodeUid = nU->cour;

				// Heuristic Cuts
				if (
					params->clients[nodeUid].demand 
					+ params->clients[nU->next->cour].demand 
					+ rV->load 
					> params->vehicleCapacity
					)
				{
					nU = nU->next;
					continue;
				};

				// Find the best position on route V
				while (!nV->isDepot || !depotTried)
				{
					depotTried = true;

					int nodeVid = nV->cour;
					if (nV->isDepot) nodeVid = nV->route->cour;

					prepareNodes(nodeUid, nodeVid, false, nV->isDepot);

					moveCheck = shift20_check();
					if (moveCheck.isValid == true && moveCheck.costChange < -MY_EPSILON) {
						if (moveCheck.costChange < bestMoveInfo.costChange || firstMove == true) {
							hadAnyImprovement = true;
							firstMove = false;
							bestMoveInfo.costChange = moveCheck.costChange;
							bestMoveInfo.nodeUcour = nodeUid;
							bestMoveInfo.nodeVcour = nodeVid;
							bestMoveInfo.isVdepot = nV->isDepot;
						}
					}
					nV = nV->next;
				}
				nU = nU->next;
			}
		}

		if (hadAnyImprovement == false) neighbStatus->setMoveRouteState(moveId, routeUid, false);
	}

	return bestMoveInfo;
}

MoveInfo RVND::swap11_sweep()
{
	MoveCheck moveCheck;
	MoveInfo bestMoveInfo;
	bool firstMove = true;
	int moveId = 2;

	for (int routeUid = 0; routeUid < routes.size() - 1; routeUid++) {
		Route* rU = &routes[routeUid];
		if (rU->nbNodes < 2) {
			neighbStatus->setMoveRouteState(moveId, routeUid, false);
			continue;
		}
		bool hadAnyImprovement = false;

		for (int routeVid = routeUid + 1; routeVid < routes.size(); routeVid++) {
			// ADS neighborhood status check heuristic
			if (neighbStatus->worthMoveBetween(moveId, routeUid, routeVid) == false) continue;

			Route* rV = &routes[routeVid];

			// Heuristic Cuts
			if (rU->min1load - rV->max1load + rV->load > params->vehicleCapacity) continue;

			// ----
			Node* nU = depots[routeUid].next;
			Node* nV = depots[routeVid].next;

			while (!nU->isDepot)
			{
				int nodeUid = nU->cour;

				// Heuristic Cuts
				if (
					params->clients[nodeUid].demand 
					+ rV->load 
					- rV->max1load
					> params->vehicleCapacity)
				{
					nU = nU->next;
					continue;
				}

				// Find the best position on route V
				while (!nV->isDepot)
				{
					int nodeVid = nV->cour;
					prepareNodes(nodeUid, nodeVid);

					moveCheck = swap11_check();
					if (moveCheck.isValid == true && moveCheck.costChange < -MY_EPSILON) {
						if (moveCheck.costChange < bestMoveInfo.costChange || firstMove == true) {
							hadAnyImprovement = true;
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

		if (hadAnyImprovement == false) neighbStatus->setMoveRouteState(moveId, routeUid, false);
	}

	return bestMoveInfo;
}

MoveInfo RVND::swap21_sweep()
{
	MoveCheck moveCheck;
	MoveInfo bestMoveInfo;
	bool firstMove = true;
	int moveId = 3;

	for (int routeUid = 0; routeUid < routes.size() - 1; routeUid++) {
		Route* rU = &routes[routeUid];
		if (rU->nbNodes < 3) {
			neighbStatus->setMoveRouteState(moveId, routeUid, false);
			continue;
		}
		bool hadAnyImprovement = false;


		for (int routeVid = 0; routeVid < routes.size(); routeVid++) {
			// ADS neighborhood status check heuristic
			if (neighbStatus->worthMoveBetween(moveId, routeUid, routeVid) == false) continue;

			Route* rV = &routes[routeVid];

			// Heuristic Cuts
			if (routeVid == routeUid) continue;
			if (rU->min2load - rV->max1load + rV->load > params->vehicleCapacity) continue;

			// ----
			Node* nU = depots[routeUid].next;
			Node* nV = depots[routeVid].next;

			while (!nU->isDepot)
			{
				int nodeUid = nU->cour;

				// Heuristic Cuts
				if (
					params->clients[nodeUid].demand
					+ params->clients[nU->next->cour].demand
					+ rV->load
					- rV->max1load
					> params->vehicleCapacity)
				{
					nU = nU->next;
					continue;
				}

				// Find the best position on route V
				while (!nV->isDepot)
				{
					int nodeUid = nU->cour;
					int nodeVid = nV->cour;
					prepareNodes(nodeUid, nodeVid);

					moveCheck = swap21_check();
					if (moveCheck.isValid == true && moveCheck.costChange < -MY_EPSILON) {
						if (moveCheck.costChange < bestMoveInfo.costChange || firstMove == true) {
							hadAnyImprovement = true;
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

		if (hadAnyImprovement == false) neighbStatus->setMoveRouteState(moveId, routeUid, false);
	}

	return bestMoveInfo;
}

MoveInfo RVND::swap22_sweep()
{
	MoveCheck moveCheck;
	MoveInfo bestMoveInfo;
	bool firstMove = true;
	int moveId = 4;

	for (int routeUid = 0; routeUid < routes.size() - 1; routeUid++) {
		Route* rU = &routes[routeUid];
		if (rU->nbNodes < 3) {
			neighbStatus->setMoveRouteState(moveId, routeUid, false);
			continue;
		}
		bool hadAnyImprovement = false;

		for (int routeVid = routeUid + 1; routeVid < routes.size(); routeVid++) {
			// ADS neighborhood status check heuristic
			if (neighbStatus->worthMoveBetween(moveId, routeUid, routeVid) == false) continue;

			Route* rV = &routes[routeVid];

			// Heuristic Cuts
			if (routeVid == routeUid) continue;
			if (rU->min2load - rV->max2load + rV->load > params->vehicleCapacity) continue;

			// ----
			Node* nU = depots[routeUid].next;
			Node* nV = depots[routeVid].next;

			while (!nU->isDepot)
			{
				int nodeUid = nU->cour;

				// Heuristic Cuts
				if (
					params->clients[nodeUid].demand
					+ params->clients[nU->next->cour].demand
					+ rV->load
					- rV->max1load
					> params->vehicleCapacity)
				{
					nU = nU->next;
					continue;
				}

				// Find the best position on route V
				while (!nV->isDepot)
				{
					int nodeUid = nU->cour;
					int nodeVid = nV->cour;
					prepareNodes(nodeUid, nodeVid);

					moveCheck = swap22_check();
					if (moveCheck.isValid == true && moveCheck.costChange < -MY_EPSILON) {
						if (moveCheck.costChange < bestMoveInfo.costChange || firstMove == true) {
							hadAnyImprovement = true;
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

		if (hadAnyImprovement == false) neighbStatus->setMoveRouteState(moveId, routeUid, false);
	}

	return bestMoveInfo;
}

MoveInfo RVND::cross_sweep()
{
	MoveCheck moveCheck;
	MoveInfo bestMoveInfo;
	bool firstMove = true;
	int moveId = 5;

	for (int routeUid = 0; routeUid < routes.size() - 1; routeUid++) {
		Route* rU = &routes[routeUid];
		if (rU->nbNodes < 2) {
			neighbStatus->setMoveRouteState(moveId, routeUid, false);
			continue;
		}
		bool hadAnyImprovement = false;

		for (int routeVid = routeUid + 1; routeVid < routes.size(); routeVid++) {
			// ADS neighborhood status check heuristic
			if (neighbStatus->worthMoveBetween(moveId, routeUid, routeVid) == false) continue;

			Route* rV = &routes[routeVid];
			Node* nU = depots[routeUid].next;
			Node* nV = depots[routeVid].next;

			while (!nU->isDepot)
			{
				int nodeUid = nU->cour;

				// Heuristic Cuts
				if (
					nU->cumulatedLoad
					+ params->clients[rU->depot->prev->prev->cour].demand
					> params->vehicleCapacity)
				{
					nU = nU->next;
					continue;
				}

				// Find the best position on route V
				while (!nV->isDepot)
				{
					int nodeUid = nU->cour;
					int nodeVid = nV->cour;
					prepareNodes(nodeUid, nodeVid);

					moveCheck = cross_check();
					if (moveCheck.isValid == true && moveCheck.costChange < -MY_EPSILON) {
						if (moveCheck.costChange < bestMoveInfo.costChange || firstMove == true) {
							hadAnyImprovement = true;
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

		if (hadAnyImprovement == false) neighbStatus->setMoveRouteState(moveId, routeUid, false);
	}

	return bestMoveInfo;
}

// -------------------------------------------------------------
// INTRA MOVE SWEEPS: search current solution for the best move
// -------------------------------------------------------------

MoveInfo RVND::reinsertion_sweep(int routeId)
{
	MoveCheck moveCheck;
	MoveInfo bestMoveInfo;
	bool firstMove = true;

	for (int routeId = 0; routeId < routes.size() - 1; routeId++) {
		Route* route = &routes[routeId];

		Node* nU = depots[routeId].next;
		while (!nU->isDepot)
		{
			Node* nV = &depots[routeId];

			bool depotTried = false;
			while (!nV->isDepot || !depotTried)
			{
				depotTried = true;
				if (nU == nV) { nV = nV->next; continue;  }

				int nodeUid = nU->cour;
				int nodeVid = nV->cour;
				if (nV->isDepot) nodeVid = routeId;

				prepareNodes(nodeUid, nodeVid, false, nV->isDepot);

				moveCheck = reinsertion_check();
				if (moveCheck.isValid == true && moveCheck.costChange <= -MY_EPSILON) {
					if (moveCheck.costChange < bestMoveInfo.costChange || firstMove == true) {
						firstMove = false;
						bestMoveInfo.costChange = moveCheck.costChange;
						bestMoveInfo.nodeUcour = nodeUid;
						bestMoveInfo.nodeVcour = nodeVid;
						bestMoveInfo.isVdepot = nV->isDepot;
					}
				}
				nV = nV->next;
			}
			nU = nU->next;
		}
	}

	return bestMoveInfo;
}

MoveInfo RVND::exchange_sweep(int routeId)
{
	MoveCheck moveCheck;
	MoveInfo bestMoveInfo;
	bool firstMove = true;

	Route* route = &routes[routeId];

	Node* nU = depots[routeId].next;
	while (!nU->isDepot)
	{
		Node* nV = nU->next;

		while (!nV->isDepot)
		{
			if (nU == nV) { nV = nV->next; continue; }

			int nodeUid = nU->cour;
			int nodeVid = nV->cour;
			prepareNodes(nodeUid, nodeVid);

			moveCheck = exchange_check();
			if (moveCheck.isValid == true && moveCheck.costChange < -MY_EPSILON) {
				if (moveCheck.costChange < bestMoveInfo.costChange || firstMove == true) {
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

	return bestMoveInfo;
}

MoveInfo RVND::oropt2_sweep(int routeId)
{
	MoveCheck moveCheck;
	MoveInfo bestMoveInfo;
	bool firstMove = true;

	Route* route = &routes[routeId];

	Node* nU = depots[routeId].next;
	while (!nU->isDepot)
	{
		Node* nV = &depots[routeId];

		bool depotTried = false;
		while (!nV->isDepot || !depotTried)
		{
			depotTried = true;
			if (nU == nV) { nV = nV->next; continue; }

			int nodeUid = nU->cour;
			int nodeVid = nV->cour;
			if (nV->isDepot) nodeVid = routeId;

			prepareNodes(nodeUid, nodeVid, false, nV->isDepot);

			moveCheck = oropt2_check();
			if (moveCheck.isValid == true && moveCheck.costChange < -MY_EPSILON) {
				if (moveCheck.costChange < bestMoveInfo.costChange || firstMove == true) {
					firstMove = false;
					bestMoveInfo.costChange = moveCheck.costChange;
					bestMoveInfo.nodeUcour = nodeUid;
					bestMoveInfo.nodeVcour = nodeVid;
					bestMoveInfo.isVdepot = nV->isDepot;
				}
			}
			nV = nV->next;
		}
		nU = nU->next;
	}

	return bestMoveInfo;
}

MoveInfo RVND::oropt3_sweep(int routeId)
{
	MoveCheck moveCheck;
	MoveInfo bestMoveInfo;
	bool firstMove = true;

	Route* route = &routes[routeId];

	Node* nU = depots[routeId].next;
	while (!nU->isDepot)
	{
		Node* nV = &depots[routeId];

		bool depotTried = false;
		while (!nV->isDepot || !depotTried)
		{
			depotTried = true;
			if (nU == nV) { nV = nV->next; continue; }

			int nodeUid = nU->cour;
			int nodeVid = nV->cour;
			if (nV->isDepot) nodeVid = routeId;

			prepareNodes(nodeUid, nodeVid, false, nV->isDepot);

			moveCheck = oropt3_check();
			if (moveCheck.isValid == true && moveCheck.costChange < -MY_EPSILON) {
				if (moveCheck.costChange < bestMoveInfo.costChange || firstMove == true) {
					firstMove = false;
					bestMoveInfo.costChange = moveCheck.costChange;
					bestMoveInfo.nodeUcour = nodeUid;
					bestMoveInfo.nodeVcour = nodeVid;
					bestMoveInfo.isVdepot = nV->isDepot;
				}
			}
			nV = nV->next;
		}
		nU = nU->next;
	}

	return bestMoveInfo;
}

MoveInfo RVND::twoopt_sweep(int routeId)
{
	MoveCheck moveCheck;
	MoveInfo bestMoveInfo;
	bool firstMove = true;

	Route* route = &routes[routeId];

	Node* nU = depots[routeId].next;
	while (!nU->isDepot)
	{
		Node* nV = nU->next;

		while (!nV->isDepot)
		{
			int nodeUid = nU->cour;
			int nodeVid = nV->cour;
			prepareNodes(nodeUid, nodeVid);

			moveCheck = twoopt_check();
			if (moveCheck.isValid == true && moveCheck.costChange < -MY_EPSILON) {
				if (moveCheck.costChange < bestMoveInfo.costChange || firstMove == true) {
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

	return bestMoveInfo;
}

// -------------------------------------------------------------
// SOLUTION FUNCTIONS
// -------------------------------------------------------------

void RVND::loadSolution(Solution & mySol)
{
	emptyRoutes.clear();
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
		updateRouteData(&routes[r],false,false);
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

RVND::RVND(Params* params, NeighbStatus * neighbStatus) : params(params) , neighbStatus(neighbStatus)
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

		case 1:
			return shift20_sweep();

		case 2:
			return swap11_sweep();

		case 3:
			return swap21_sweep();

		case 4:
			return swap22_sweep();

		case 5:
			return cross_sweep();

		default:
			return moveInfo;
	}
}

bool RVND::doInterMoveOnCurrentSolution(int moveId, MoveInfo & moveInfo)
{
	prepareNodes(moveInfo.nodeUcour, moveInfo.nodeVcour, false, moveInfo.isVdepot);

	switch (moveId)
	{
	case 0:
		return shift10_do();

	case 1:
		return shift20_do();

	case 2:
		return swap11_do();

	case 3:
		return swap21_do();

	case 4:
		return swap22_do();

	case 5:
		return cross_do();

	default:
		return false;
	}
}

// --------------
void RVND::doIntraRouteOptimizationOnSolution() {
	for (int i = 0; i < routes.size(); i++) {
		Route* route = &routes[i];
		if (route->nbNodes < 3) continue;
		doIntraRouteOptimizationOnRoute(i);
	}
}

bool RVND::doIntraRouteOptimizationOnRoute(int routeId)
{
	int moveId = 0;
	MoveInfo moveInfo;

	populateIntraRouteNL();
	while (intraRouteNL.size() > 0)
	{
		moveId = intraRouteNL.back();

		moveInfo = getBestIntraRouteMove(moveId, routeId);
		bool moveDone = false;
		bool moveTryed = false;
		if (moveInfo.costChange <= -MY_EPSILON)
		{
			moveTryed = true;
			moveDone = doIntraMoveOnCurrentSolution(moveId, moveInfo);
		}
		if (moveDone == false) {
			intraRouteNL.pop_back();
		}
	}
}


void RVND::populateIntraRouteNL()
{
	// Init list
	intraRouteNL = std::vector<int>(INTRAROUTE_MOVES);
	for (int i = 0; i < INTRAROUTE_MOVES; i++) intraRouteNL.push_back(i);

	// Shuffle list
	std::shuffle(intraRouteNL.begin(), intraRouteNL.end(), params->generator);
}

MoveInfo RVND::getBestIntraRouteMove(int moveId, int routeId)
{
	MoveInfo moveInfo;
	switch (moveId)
	{
	case 0:
		return reinsertion_sweep(routeId);

	case 1:
		return exchange_sweep(routeId);

	case 2:
		return oropt2_sweep(routeId);

	case 3:
		return oropt3_sweep(routeId);

	case 4:
		return twoopt_sweep(routeId);

	default:
		return moveInfo;
	}
}

bool RVND::doIntraMoveOnCurrentSolution(int moveId, MoveInfo& moveInfo)
{
	prepareNodes(moveInfo.nodeUcour, moveInfo.nodeVcour, false, moveInfo.isVdepot);
	
	switch (moveId)
	{
	case 0:
		return reinsertion_do();

	case 1:
		return exchange_do();

	case 2:
		return oropt2_do();

	case 3:
		return oropt3_do();

	case 4:
		return twoopt_do();

	default:
		return false;
	}
}

void RVND::populatePerturbRouteNL()
{
	// Init list
	perturbRouteNL = std::vector<int>(PERTURBROUTE_MOVES);
	for (int i = 0; i < PERTURBROUTE_MOVES; i++) perturbRouteNL.push_back(i);

	// Shuffle list
	std::shuffle(perturbRouteNL.begin(), perturbRouteNL.end(), params->generator);
}
