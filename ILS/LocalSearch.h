#ifndef LOCALSEARCH_H
#define LOCALSEARCH_H

#include "Solution.h"

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
};

class LocalSearch
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

	// Calcumating load and duration penalty calculations (for methods that allow infeasible solutions in terms of capacity)
	double excessCharge(double myCharge) {return std::max<double>(0., myCharge - params->vehicleCapacity)*params->penalityCapa;}

	/* RELOCATE MOVES */
	bool move1 (); // If U is a client node, remove U and insert it after V
	// ...

	/* SWAP MOVES */
	bool move4(); // If U and V are client nodes, swap U and V
	// ...

	/* 2-OPT and 2-OPT* MOVES */
	bool move7(); // If route(U) == route(V), replace (U,X) and (V,Y) by (U,V) and (X,Y)
	bool move8(); // If route(U) != route(V), replace (U,X) and (V,Y) by (U,V) and (X,Y)

	// Intra Route Moves
	bool reinsertion();
	bool exchange();
	bool oropt2();
	bool oropt3();
	bool twoopt();

	// Inter Route Moves
	bool shift10();
	bool shift20();
	bool shift30();
	bool swap11();
	bool swap21();
	bool swap22();
	bool cross();

	
	/* ROUTINES TO UPDATE THE DATA STUCTURES REPRESENTING THE SOLUTIONS */
	static void insertNode(Node * U, Node * V);			// Solution update: Insert U after V
	static void insertNode2(Node* U, Node* V);			// Solution update: Insert U and its next node after V
	static void insertNode3(Node* U, Node* V);			// Solution update: Insert U and its two next nodes after V
	static void swapNode(Node * U, Node * V) ;			// Solution update: Swap U with V							   
	static void swapNode2(Node * U, Node * V) ;			// Solution update: Swap (U and U->next) with V							   
	static void swapNode22(Node * U, Node * V) ;		// Solution update: Swap (U and U->next) with (V and V->next)							   
	void updateRouteData(Route * myRoute);	// Updates the preprocessed data of a route

	// Loading an initial solution into the local search structures
	void loadSolution(Solution & mySol);

	// Exporting the result as a solution
	void exportSolution(Solution & mySol);

	public:

	// Run the local search on a given solution
	void run(Solution & mySol);

	// Run the local search on a given solution
	void runTests(Solution& mySol, std::string exportFile);
	void exportTestToFile(std::vector < std::string > & results, std::string exportFile);
	void routesToString(std::string header, std::vector < std::string >& results);
	void prepareNodes(int nudeIdU, int nudeIdV);

	// Constructor
	LocalSearch(Params * params);
};

#endif
