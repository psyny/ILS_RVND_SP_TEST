#ifndef ILSRVND_H
#define ILSRVND_H

#include "Solution.h"

class ILSRVND
{

private:
	Params * params ;							// Problem parameters

public:
	// Run the local search on a given solution
	void run(Solution & mySol);

	// Tests
	void runTests(Solution & mySol, std::string exportFile);

	// Constructor
	ILSRVND(Params * params);
};

#endif
