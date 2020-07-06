#ifndef RVND_H
#define RVND_H

#include "Solution.h"

#define RVND_DEBUG false

#define INTERROUTE_MOVES 6
#define INTRAROUTE_MOVES 5
#define PERTURBROUTE_MOVES 2
#define PERTURBROUTE_MIN 1
#define PERTURBROUTE_MAX 3
#define PERTURBROUTE_MAXFAILS 10

struct Node;

struct Route
{
	int cour;					// Route index
	int nbNodes;				// Number of nodes in the route
	Node * depot;				// Pointer to the depot node
	double load;				// Total load on the route
	double min1load;					// Min load of a single customer on this route
	double max1load;					// Max load of a single customer on this route
	double min2load;					// Min load of a two adjacents customers on this route
	double max2load;					// Max load of a two adjacents customers on this route
	double min3load;					// Min load of a three adjacents customers on this route
	double max3load;					// Max load of a three adjacents customers on this route
	double totalCost{ 0 };
};

struct Node
{
	int cour;							// Node index
	bool isDepot;						// Tells whether this node represents a depot or not
	int position;						// Position in the route
	Node * next;						// Next node in the route order
	Node * prev;						// Previous node in the route order
	Route * route;						// Pointer towards the associated route
	double cumulatedLoad;				// Cumulated load on this route until the customer (including itself)
	double cumulatedCost{ 0 };
};

struct MoveInfo
{
	int nodeUcour{ 0 };
	int nodeVcour{ 0 };
	bool isVdepot{ false };
	double costChange{ 0 };
};

struct MoveCheck
{
	bool isValid{ false };
	double costChange{ 0 };
};

class RVND
{

private:
	
	Params * params ;							// Problem parameters
	bool searchCompleted;						// Tells whether all moves have been evaluated without success
	long long int nbMoves;					    // Number of moves applied during the local search
	std::vector<int> orderNodes;				// Used to randomize the move evaluations in the local search
	std::unordered_set < int > emptyRoutes;		// indices of all empty routes

	/* THE SOLUTION IS REPRESENTED AS A LINKED LIST OF ELEMENTS */
	std::vector < Node > clients;				// Elements representing clients (clients[0] is a sentinel and should not be accessed)
	std::vector < Node > depots;				// Elements representing depots
	std::vector < Node > depotsEnd;				// Duplicate of the depots to mark the end of the routes
	std::vector < Route > routes;				// Elements representing routes

	/* TEMPORARY VARIABLES USED IN THE LOCAL SEARCH LOOPS */
	Node * nodeU ;
	Node * nodeX ;
    Node * nodeV ;
	Node * nodeY ;
	Route * routeU ;
	Route * routeV ;
	int nodeUPredCour, nodeUCour, nodeXCour, nodeXSuivCour ;	// UPred -> U -> X -> XSuiv
	int nodeVPredCour, nodeVCour, nodeYCour, nodeYSuivCour ;	// VPred -> V -> Y -> YSuiv
	double loadU, loadX, loadV, loadY; // Some temporary variables for an easier access to the demands
	void setLocalVariablesRouteU(); // Initializes some local variables and distances associated to routeU
	void setLocalVariablesRouteV(); // Initializes some local variables and distances associated to routeV

	/* Neighborhood Lists */
	std::vector <int> interRouteNL;
	std::vector <int> intraRouteNL;
	std::vector <int> perturbRouteNL;

	// ---------------------------------------------

	/* RELOCATE MOVES */
	// General Move Functions

	// Inter Route Moves
	MoveInfo getBestInterRouteMove(int moveId);
	bool doInterMoveOnCurrentSolution(int moveId, MoveInfo& moveInfo);

	MoveInfo shift10_sweep();
	MoveCheck shift10_check();
	bool shift10_do();

	MoveInfo shift20_sweep();
	MoveCheck shift20_check();
	bool shift20_do();

	MoveCheck shift30_check();
	bool shift30_do();

	MoveInfo swap11_sweep();
	MoveCheck swap11_check();
	bool swap11_do();

	MoveInfo swap21_sweep();
	MoveCheck swap21_check();
	bool swap21_do();

	MoveInfo swap22_sweep();
	MoveCheck swap22_check();
	bool swap22_do();

	MoveInfo cross_sweep();
	MoveCheck cross_check();
	bool cross_do();

	// Intra Route Moves
	MoveInfo getBestIntraRouteMove(int moveId);
	bool doIntraMoveOnCurrentSolution(int moveId, MoveInfo& moveInfo);

	MoveInfo reinsertion_sweep();
	MoveCheck reinsertion_check();
	bool reinsertion_do();

	MoveInfo exchange_sweep();
	MoveCheck exchange_check();
	bool exchange_do();

	MoveInfo oropt2_sweep();
	MoveCheck oropt2_check();
	bool oropt2_do();

	MoveInfo oropt3_sweep();
	MoveCheck oropt3_check();
	bool oropt3_do();

	MoveInfo twoopt_sweep();
	MoveCheck twoopt_check();
	bool twoopt_do();


	/* ROUTINES TO UPDATE THE DATA STUCTURES REPRESENTING THE SOLUTIONS */
	static void insertNode(Node * U, Node * V);			// Solution update: Insert U after V
	static void insertNode2(Node* U, Node* V);			// Solution update: Insert U and its next node after V
	static void insertNode3(Node* U, Node* V);			// Solution update: Insert U and its two next nodes after V
	static void swapNode11(Node * U, Node * V) ;			// Solution update: Swap U with V							   
	static void swapNode21(Node * U, Node * V) ;			// Solution update: Swap (U and U->next) with V							   
	static void swapNode22(Node * U, Node * V) ;			// Solution update: Swap (U and U->next) with (V and V->next)							   
	static void crossNode(Node* U, Node* V);				// Solution update: Cross nodes: U->next = Y and V->next = X
	static void crossNodeSameRoute(Node* U, Node* V);		// Solution update: Cross nodes: U->next = Y and V->next = X (same route)
	void updateRouteData(Route * myRoute, bool updateNodeRouteInfo = false);	// Updates the preprocessed data of a route

	// Loading an initial solution into the local search structures
	void loadSolution(Solution & mySol);

	// Exporting the result as a solution
	void exportSolution(Solution & mySol);

	/* Neighborhood Lists */
	void populateInterRouteNL();
	void populateIntraRouteNL();
	void populatePerturbRouteNL();


public:
	// Run the local search on a given solution
	void run(Solution & mySol);

	// Run the pertubations mechanic
	void perturb(Solution& mySol);

	// Run the local search on a given solution
	void runTests(Solution& mySol, std::string exportFile);
	void exportTestToFile(std::vector < std::string > & results, std::string exportFile);
	void routesToString(std::string header, std::vector < std::string >& results);
	void prepareNodes(int nodeIdU, int nodeIdV, bool isUdepot = false, bool isVdepot = false);

	// Constructor
	RVND(Params * params);

	bool isPerturb = false;
};

#endif
