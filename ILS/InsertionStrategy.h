#ifndef ILS_INSERTIONSTRATEGY_H
#define ILS_INSERTIONSTRATEGY_H

#include "Solution.h"
#include "vector"
#include "InsertionCriterion.h"
#include "InsertionCriterion.h"

class InsertionStrategy {
public:
    virtual void execute(Params *params, Solution *s, std::vector<int> *cl, InsertionCriterion *insertionCriterion) = 0;
};

#endif //ILS_INSERTIONSTRATEGY_H
